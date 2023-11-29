
#include <aven/GL/Program.h>
#include <aven/GL/ShaderLoader.h>
#include <aven/GL/SSBO.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <memory>


namespace aven {

	namespace {
		std::unique_ptr<gl::SSBO>		ssbo_raycast;

		std::unique_ptr<gl::Program>	prog_raycast;
		std::unique_ptr<gl::Program>	prog_blend;
		std::unique_ptr<gl::Program>	prog_paint_mask;
		std::unique_ptr<gl::Program>	prog_paintShape;
	}


	namespace volumeOps {
	
		void init() {
			ssbo_raycast		= std::make_unique<gl::SSBO>(4*4);
			prog_raycast		= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/raycast.glsl"} }));
			prog_blend			= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/blendVolumes.glsl"} }));
			prog_paint_mask		= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/paintMask.glsl"} }));
			prog_paintShape		= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/paintShape.glsl"} }));
		}


		void destroy() {
			prog_paintShape.reset();
			prog_paint_mask.reset();
			prog_blend.reset();
			prog_raycast.reset();
			ssbo_raycast.reset();
		}


		ivec3 raycast(gl::Texture3D_rgba8u const& tex, Ray const& ray) {	
			assert(prog_raycast);
			assert(ssbo_raycast);

			tex.bindToImageTextureUnit_readOnly(0);
			prog_raycast->setInt3("volume_size",	tex.getSize());
			prog_raycast->setVec3("ray_pos",		ray.pos);
			prog_raycast->setVec3("ray_dir",		ray.dir);

			ssbo_raycast->bindBufferBase(6);
			gl::dispatch(*prog_raycast, 1, 1, 1);

			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			ivec3 pos;
			ssbo_raycast->getDataRange(&pos[0], 0, 3*4);

			return pos;
		}


		gl::Texture3D_rgba8u paint_mask(gl::Texture3D_rgba8u const& src, gl::Texture3D_r8u const& mask, vec3 color, float opacity) {
			assert(src.getSize() == mask.getSize());
			assert(prog_paint_mask);
			
			auto size = src.getSize();
			auto dst = gl::Texture3D_rgba8u(size);

			src.bindToImageTextureUnit_readOnly(0);
			mask.bindToImageTextureUnit_readOnly(1);
			dst.bindToImageTextureUnit_writeOnly(2);

			prog_paint_mask->setInt3("volume_size", size);
			prog_paint_mask->setFloat("opacity", opacity);
			prog_paint_mask->setVec3("color", color);
			prog_paint_mask->setInt("blendMode", 0);
			
			ivec3 nbrGroups = (size + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
			gl::dispatch(*prog_paint_mask, nbrGroups.x, nbrGroups.y, nbrGroups.z);
			
			return dst;
		}



		gl::Texture3D_rgba8u erase_mask(gl::Texture3D_rgba8u const& src, gl::Texture3D_r8u const& mask, float opacity) {
			assert(src.getSize() == mask.getSize());
			assert(prog_paint_mask);
			
			auto size = src.getSize();
			auto dst = gl::Texture3D_rgba8u(size);

			src.bindToImageTextureUnit_readOnly(0);
			mask.bindToImageTextureUnit_readOnly(1);
			dst.bindToImageTextureUnit_writeOnly(2);

			prog_paint_mask->setInt3("volume_size", size);
			prog_paint_mask->setFloat("opacity", opacity);
			prog_paint_mask->setInt("blendMode", 1);
			
			ivec3 nbrGroups = (size + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
			gl::dispatch(*prog_paint_mask, nbrGroups.x, nbrGroups.y, nbrGroups.z);
			
			return dst;
		}


		void paint(gl::Texture3D_r8u& tex, ivec3 pos, int radius, Brush mask, float color) {
			assert(prog_paintShape);
			
			AABB<int> aabb = { pos - radius, pos + radius };
			auto intersection = intersect(AABB<int>({0,0,0}, tex.getSize()-1), aabb);
			if (!intersection || intersection->isEmpty())
				return;
			
			tex.bindToImageTextureUnit(1);
			prog_paintShape->setVec4("color", color);
			prog_paintShape->setInt3("aabb_intersection_min", intersection->getMin());
			prog_paintShape->setInt3("aabb_intersection_max", intersection->getMax());
			prog_paintShape->setInt3("pos_center", pos);
			prog_paintShape->setInt("radius", radius);
			prog_paintShape->setInt("shape", static_cast<int>(mask));
			

			ivec3 nbrGroups = (intersection->getSize() + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
			gl::dispatch(*prog_paintShape, nbrGroups.x, nbrGroups.y, nbrGroups.z);
		}


		void paintMirror(gl::Texture3D_r8u& tex, ivec3 posBrush, int radius, Brush brush, float color, bvec3 mirrored){
			auto mirrorIterations = ivec3(mirrored.x ? 2 : 1, mirrored.y ? 2 : 1, mirrored.z ? 2 : 1);
			for (int i = 0; i < mirrorIterations.x; i++) {
				for (int j = 0; j < mirrorIterations.y; j++) {
					for (int k = 0; k < mirrorIterations.z; k++) {
						ivec3 texSize = tex.getSize();
						ivec3 pos = ivec3(	i == 0 ? posBrush.x : texSize.x - posBrush.x, 
											j == 0 ? posBrush.y : texSize.y - posBrush.y,
											k == 0 ? posBrush.z : texSize.z - posBrush.z);

						paint(tex, pos, radius, brush, color);
					}
				}
			}
		}


		void paintStroke_Mirror(gl::Texture3D_r8u& tex, ivec3 from, ivec3 to, int nbrIterations, int radius, Brush brush, float color, bvec3 mirrored) {
			vec3 dir = vec3(to - from);

			for (int it = 0; it < nbrIterations; it++) {
				float t = static_cast<float>(it) / static_cast<float>(nbrIterations);
				ivec3 pos = ivec3(vec3(from) + dir * t);		

				paintMirror(tex, pos, radius, brush, color, mirrored);
			}		
		}


	}
}		