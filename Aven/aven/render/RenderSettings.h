#pragma once

#include <acore/clamped.h>
#include <expected>
#include <fstream>

namespace aven {
	struct RenderSettings {
		c_float<0.0001f,10.0f>	gamma					{1.5f};
		c_int<1,10000>			maxSamples				{200};
		c_int<1,32>				nbrSamplesPerIteration	{1};
		c_int<0,16>				nbrBounces				{0};
		c_vec3<0,1>				backgroundColor_sky		{vec3(0.227f, 0.345f, 0.513f)};
		c_vec3<0,1>				backgroundColor_ground	{vec3(0.0f, 0.0f, 0.0f)};
		bool					useBackgroundAsLight	{ true };	
		c_float<0.0f, 10000.0f>	backgroundIntensity		{ 10 };	
			
		static void serialize(std::ofstream&, RenderSettings const&);
		static std::expected<RenderSettings, std::string> deserialize(std::ifstream&);

	};
}
