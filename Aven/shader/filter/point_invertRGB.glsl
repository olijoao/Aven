#version 460 core

#define FILTER_COLOR
#pragma include "shader/lib/filter_main.glsl"

vec3 filter_color(in vec3 color) {
	return 1.0f - color;
}