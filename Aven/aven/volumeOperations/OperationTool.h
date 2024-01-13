#pragma once

#include <aven/objects/VolumeData.h>
#include <aven/util/clamped.h>
#include <aven/volumeOperations/Tool_Brush.h>
#include <aven/volumeOperations/VolumeOps.h>
#include <optional>

namespace aven{
	class Tool_Brush;

	class OperationTool {
	public:
		OperationTool(Tool_Brush* tool, ivec3 size, clamped<int, 1, 255> opacity, volumeOps::BlendMode blendMode);

		VolumeData					volumeData;
		Tool_Brush*	const			tool;
		std::optional<ivec3>		brushStroke_lastPos{};
		float						brushStroke_distanceRemaining{ 0.0f };
		clamped<int, 1, 255> const	opacity;
		volumeOps::BlendMode const	blendMode;

	private:
		//..
	};
}