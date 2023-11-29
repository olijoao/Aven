#pragma once

#include <aven/util/geo3d.h>
#include <aven/GL/Texture.h>
#include <aven/volumeOperations/Brush.h>

namespace aven {
	namespace volumeOps {

		void init();
		void destroy();

		// query
		ivec3 raycast(gl::Texture3D_rgba8u const& tex, Ray const& ray);

		// paint mask, return new
		gl::Texture3D_rgba8u paint_mask(gl::Texture3D_rgba8u const& src, gl::Texture3D_r8u const& mask, vec3 color, float opacity);
		gl::Texture3D_rgba8u erase_mask(gl::Texture3D_rgba8u const& src, gl::Texture3D_r8u const& mask, float opacity);


		// in place painting
		void paint(gl::Texture3D_r8u&, ivec3 pos, int radius, Brush, float color);
		void paintMirror(gl::Texture3D_r8u&, ivec3 pos, int radius, Brush, float color, bvec3 mirror);
		void paintStroke_Mirror(gl::Texture3D_r8u&, ivec3 from, ivec3 to, int nbrIterations, int radius, Brush, float color, bvec3 mirror);


	}
}