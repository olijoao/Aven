#pragma once

#include <aven/util/geo3d.h>
#include <aven/util/math.h>


namespace aven {
	class MouseInput {
	public:
		MouseInput(vec2 pos, Ray ray);
		vec2 pos;	//in [0,1]	
		Ray ray;
	};
}