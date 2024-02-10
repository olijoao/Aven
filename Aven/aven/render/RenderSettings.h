#pragma once

#include <aven/util/clamped.h>
#include <expected>
#include <fstream>

namespace aven {
	struct RenderSettings {
		c_float<0.0001f,10.0f>	gamma					{2};
		c_int<1,1000>			maxSamples				{100};
		c_int<1,32>				nbrSamplesPerIteration	{2};
		c_int<0,16>				nbrBounces				{0};
		c_vec3<0,1>				backgroundColor_sky		{vec3(0.2f, 0.2f, 0.2f)};
		c_vec3<0,1>				backgroundColor_ground	{vec3(0.0f, 0.0f, 0.0f)};

		static void serialize(std::ofstream&, RenderSettings const&);
		static std::expected<RenderSettings, std::string> deserialize(std::ifstream&);

	};
}
