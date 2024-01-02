#pragma once

#include <aven/objects/VolumeData.h>
#include <aven/util/clamped.h>
#include <aven/volumeOperations/Tool.h>
#include <optional>


namespace aven{

	enum class BlendMode { Normal, Erase };


	class OperationTool {
	public:
		OperationTool(Tool* tool, ivec3 size, clamped<int, 1, 255> opacity, BlendMode blendMode);

	public:
		VolumeData volumeData;
		
		Tool const*			 const	tool;

		std::optional<ivec3>		brushStroke_lastPos{};
		float						brushStroke_distanceRemaining{ 0.0f };
		
		clamped<int, 1, 255> const	opacity;

		BlendMode			 const	blendMode;

	private:
		//..
	};
}