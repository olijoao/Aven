#pragma once

#include <aven/GL/Texture.h>
#include <aven/util/clamped.h>
#include <aven/volumeOperations/Tool.h>
#include <optional>


namespace aven{

	enum class BlendMode { Normal, Erase };


	class OperationTool {
	public:
		OperationTool(Tool* tool, ivec3 size, vec3 color, clamped<int, 1, 255> opacity, BlendMode blendMode);

	public:
		gl::Texture3D_r8u			texture_mask;
		Tool const*			 const	tool;

		std::optional<ivec3>		brushStroke_lastPos{};
		float						brushStroke_distanceRemaining{ 0.0f };
		
		vec3				 const	color;
		clamped<int, 1, 255> const	opacity;

		BlendMode			 const	blendMode;

	private:
		//..
	};
}