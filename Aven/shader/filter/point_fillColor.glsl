#version 460 core
#define FILTER_COLOR_ALPHA
#include "shader/lib/filter_main.glsl"

uniform vec3	color;
uniform bool	mask;

vec4 filter_color_alpha(vec4 c) {
	if (!mask)
		return vec4(color, 1);
	return vec4(color, c.a);
}



