#pragma once

#include <aven/volumeOperations/Brush.h>
#include <aven/util/clamped.h>

namespace aven {
	struct ToolBrushProperties {
		Brush							brush{ Brush::Sphere };
		clamped<int, 1, 128>			radius{ 16 };
		clamped<int, 1, 255>			flow{ 255 };
		clamped<int, 1, 255>			opacity{ 255 };
		bvec3							mirrored{ false, false, false };
		clamped<float, 0.001f, 10.0f>	spacing{ 0.1f };	// can not be 0
	};
}