#version 460 core
#define FILTER_RENDER
#include "shader/lib/filter_main.glsl"
#include "shader/lib/sample.glsl"


vec4 filter_render(ivec3 pos) {
	srand( float(pos.x + pos.y*256 + pos.z*65536));
	return vec4(rand(), rand(), rand(), 1);
}
