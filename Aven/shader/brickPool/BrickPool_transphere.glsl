#version 460 core

#include "shader/brickPool/BrickPool.glsl"

layout(std430, binding = 1) buffer TransphereBuffer{
	uint data[];
} transphere;

uniform unsigned int count;
uniform bool isAllocating;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	if (isAllocating) {
		for (int i = 0; i < count; i++)
			transphere.data[i] = brickPool_alloc();
			
	} else {
		for (int i = 0; i < count; i++)
			brickPool_free(transphere.data[i]);
	}
}

