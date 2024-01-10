#pragma once

#include <aven/volumeOperations/Brush.h>
#include <aven/util/clamped.h>

namespace aven {
	struct ToolBrushProperties {
		// tool properties
		clamped<int, 1, 255>			opacity{ 255 };
		bvec3							mirrored{ false, false, false };
		clamped<float, 0.001f, 10.0f>	spacing{ 0.1f };	// can not be 0
		
		// stamp properties
		clamped<int, 1, 128>			radius{ 16 };
		Brush							brush{ Brush::Sphere };
		clamped<int, 1, 255>			flow{ 255 };
		clamped<int, 0, 1000>			jitter_pos;
		clamped<int, 0, 1000>			jitter_size;
		clamped<int, 0, 255>			jitter_color;
		clamped<int, 0, 255>			jitter_flow;
	};

			
}