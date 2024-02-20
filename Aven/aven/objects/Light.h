#pragma once

#include <aven/util/Transformation.h>
#include <aven/util/clamped.h>
#include <aven/util/math.h>
#include <expected>
#include <fstream>


namespace aven {
	struct Light{

		enum class Type:int { Direction=0, Point=1, SpotLight=2};

		Transformation			transformation;
		Type					type					{ Type::Direction };
		c_vec3<0, 1>			color					{ 1 };
		c_float<0.f, 1000000.f> intensity				{ 1 };
		c_float<0.1f, 90.0f>	falloff_angle_degrees	{ 45.0f };	// only used by spotlight

		static void serialize(std::ofstream& out, Light const& rs);
		static std::expected<Light, std::string> deserialize(std::ifstream& in);

	};
}



