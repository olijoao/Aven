#pragma once

#include <aven/GL/SSBO.h>
#include <aven/objects/VolumeData.h>
#include <aven/util/geo3d.h>
#include <aven/volumeOperations/Brush.h>
#include <aven/volumeOperations/ToolBrushProperties.h>

namespace aven {
	namespace volumeOps {
		enum class BlendMode : int {Normal = 0, Erase = 1};

		void init();
		void destroy();

		// query
		ivec3 raycast(VolumeData const&, Ray const& ray);
	
		VolumeData blend(VolumeData const& src, VolumeData && dst, BlendMode blendmode, float opacity);
		
		// in place painting
		void paint(VolumeData&, ivec3 pos, vec4 color, ToolBrushProperties const& properties); 
		void paintStroke(VolumeData&, ivec3 from, ivec3 to, int nbrIterations, vec4 color, ToolBrushProperties const& properties);

	}
}