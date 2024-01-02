#pragma once

#include <aven/util/geo3d.h>
#include <aven/GL/SSBO.h>
#include <aven/volumeOperations/Brush.h>
#include <aven/objects/VolumeData.h>


namespace aven {
	namespace volumeOps {
		enum class BlendMode : int {Normal = 0, Erase = 1};

		void init();
		void destroy();

		// query
		ivec3 raycast(VolumeData const&, Ray const& ray);
	
		VolumeData blend(VolumeData const& src, VolumeData && dst, BlendMode blendmode, float opacity);
		
		// in place painting
		void paint(VolumeData& volumeData, ivec3 pos, int radius, Brush, vec4 color);
		void paintMirror(VolumeData& volumeData, ivec3 pos, int radius, Brush, vec4 color, bvec3 mirror);
		void paintStroke_Mirror(VolumeData& volumeData, ivec3 from, ivec3 to, int nbrIterations, int radius, Brush, vec4 color, bvec3 mirror);


	}
}