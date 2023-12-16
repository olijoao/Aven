#version 460 core
#define FILTER_RENDER
#pragma include "shader/lib/filter_main.glsl"
#pragma include "shader/lib/sample.glsl"


vec4 filter_render(ivec3 pos) {
	srand( float(pos.x + pos.y*256 + pos.z*65536));	//todo crap with different volume size
	return vec4(rand(), rand(), rand(), 1);
}
