#version 460 core

#define FILTER_DEFORM
#include "shader/lib/filter_main.glsl"

uniform vec3 scale;
uniform ivec3 translate;

ivec3 filter_deform(in ivec3 pos) {
	return ivec3(vec3(pos)/scale) - translate;
}