
#include <aven/GL/Program.h>
#include <aven/GL/ShaderLoader.h>
#include <aven/GL/SSBO.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <aven/objects/BrickPool.h>
#include <memory>
#include <random>


namespace aven {

	namespace {
		std::unique_ptr<gl::SSBO>		ssbo_raycast;

		std::unique_ptr<gl::Program>	prog_raycast;
		std::unique_ptr<gl::Program>	prog_blendVolumes;
		std::unique_ptr<gl::Program>	prog_paintShape;
	}


	namespace volumeOps {
	
		void init() {
			ssbo_raycast		= std::make_unique<gl::SSBO>(4*4);
			prog_raycast		= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/raycast.glsl"} }));
			prog_blendVolumes	= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/blendVolumes.glsl"} }));
			prog_paintShape		= std::make_unique<gl::Program>(gl::loadProgram({ {gl::ShaderType::Compute, "shader/volumeOps/paintShape.glsl"} }));
		}


		void destroy() {
			prog_paintShape.reset();
			prog_blendVolumes.reset();
			prog_raycast.reset();
			ssbo_raycast.reset();
		}


		ivec3 raycast(VolumeData const& data, Ray const& ray) {
			assert(prog_raycast);
			assert(ssbo_raycast);
		
			brickPool::bindSSBO_toBufferBase0();
			data.getSSBO().bindBufferBase(1);
			prog_raycast->setInt3("volume_size",	data.getSize());
			prog_raycast->setVec3("ray_pos",		ray.pos);
			prog_raycast->setVec3("ray_dir",		ray.dir);
		
			ssbo_raycast->bindBufferBase(6);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			gl::dispatch(*prog_raycast, 1, 1, 1);
				
			ivec3 pos;
			ssbo_raycast->getDataRange(&pos[0], 0, 3*4);
			return pos;
		}


		VolumeData blend(VolumeData const& src, VolumeData&& dst, BlendMode blendmode, float opacity){
			assert(src.getSize() == dst.getSize());
			assert(prog_blendVolumes);
			
			auto size = src.getSize();
			
			brickPool::bindSSBO_toBufferBase0();
			src.getSSBO().bindBufferBase(1);
			dst.getSSBO().bindBufferBase(2);

			prog_blendVolumes->setInt3("volume_size", size);
			prog_blendVolumes->setFloat("opacity", opacity);
			prog_blendVolumes->setInt("blendMode", static_cast<int>(blendmode));
			
			ivec3 nbrGroups = (size + ivec3(7, 7, 7)) / ivec3(8, 8, 8);
			prog_blendVolumes->setInt3("nbrGroups", nbrGroups);
			glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
			gl::dispatch(*prog_blendVolumes, 1,1,1);
			return dst;
		}


		

		void paint(VolumeData& volumeData, ivec3 pos, vec4 color, ToolBrushProperties const& props) {
			assert(prog_paintShape);
	
			std::random_device dev;
			std::mt19937 rng(dev());
		
			if (props.jitter_pos.getValue() > 0) {
				auto jitter_pos = props.jitter_pos.getValue();
				std::uniform_int_distribution<int> dist(-jitter_pos, jitter_pos);
				pos += ivec3(dist(rng), dist(rng), dist(rng));
			}

			int radius = props.radius.getValue();
			if (props.jitter_size.getValue() > 0) {
				auto jitter_size = props.jitter_size.getValue();
				std::uniform_int_distribution<int> dist(-jitter_size, jitter_size);
				radius = clamp(radius + dist(rng), 1, props.radius.getMax());
			}

			if (props.jitter_color.getValue() > 0) {
				auto jitter_color = props.jitter_color.getValue();
				std::uniform_int_distribution<int> dist(-jitter_color, jitter_color);
				vec3 col = vec3(color.x, color.y, color.z);
				col = clamp(col + vec3(dist(rng), dist(rng), dist(rng))/255, vec3(0), vec3(1));
				color = vec4(col.x, col.y, col.z, color.w);
			}
	
			if (props.jitter_flow.getValue() > 0) {
				auto jitter_flow = props.jitter_flow.getValue();
				std::uniform_int_distribution<int> dist(-jitter_flow, jitter_flow);
				color.w = clamp(color.w + static_cast<float>(dist(rng))/255, 0.0f, 1.0f);
			}
 
			volumeData.getSSBO().bindBufferBase(2);
			prog_paintShape->setVec4("color", color);
			prog_paintShape->setInt("radius", radius);
			prog_paintShape->setInt("shape", static_cast<int>(props.brush));

			ivec3 nbrGroupsInVolume = (volumeData.getSize()+7)/8;
			prog_paintShape->setInt3("nbrGroupsInVolume", nbrGroupsInVolume);

		 	ivec3 texSize = volumeData.getSize();
			auto mirrorIterations = ivec3(props.mirrored.x ? 2 : 1, props.mirrored.y ? 2 : 1, props.mirrored.z ? 2 : 1);
		 	for (int i = 0; i < mirrorIterations.x; i++) {
		 		for (int j = 0; j < mirrorIterations.y; j++) {
		 			for (int k = 0; k < mirrorIterations.z; k++) {
		 				ivec3 posCenter = ivec3(i == 0 ? pos.x : texSize.x - pos.x, 
		 										j == 0 ? pos.y : texSize.y - pos.y,
		 										k == 0 ? pos.z : texSize.z - pos.z);
						
						AABB<int> aabb = { posCenter - radius, posCenter + radius }; 
						auto intersection = intersect(AABB<int>({0,0,0}, volumeData.getSize()-1), aabb);
						if (!intersection || intersection->isEmpty())
							continue;					
						
						ivec3 pos_minBrick	= intersection->getMin()/8;
						ivec3 pos_maxBrick	= intersection->getMax()/8;
						prog_paintShape->setInt3("posBrickMin", pos_minBrick);
						prog_paintShape->setInt3("posBrickMax", pos_maxBrick);
						prog_paintShape->setInt3("aabb_intersection_min", intersection->getMin());
						prog_paintShape->setInt3("aabb_intersection_max", intersection->getMax());
						prog_paintShape->setInt3("pos_center", posCenter);
								
						glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
						gl::dispatch(*prog_paintShape, 1, 1, 1); 
		 			}
		 		}
		 	}
		}


			 
		 
		void paintStroke(VolumeData& volumeData, ivec3 from, ivec3 to, int nbrIterations, vec4 color, ToolBrushProperties const& props ) {
			vec3 dir = vec3(to - from);
		
			for (int it = 0; it < nbrIterations; it++) {
				float t = static_cast<float>(it) / static_cast<float>(nbrIterations);
				ivec3 pos = ivec3(vec3(from) + dir * t);		
		
				paint(volumeData, pos, color, props);
			}		
		}
	
	}
}		