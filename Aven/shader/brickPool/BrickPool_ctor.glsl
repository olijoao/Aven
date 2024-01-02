#version 460 core

#pragma include "shader/brickPool/BrickPool.glsl"

uniform unsigned int capacity;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	brickPool.countFree	= capacity;
	brickPool.capacity	= capacity;

	brickPool.ptr_nextFree = 0;

	for (int i = 0; i < capacity-1; i++)
		brickPool.data[i*BRICK_SIZE] = i+1;
	brickPool.data[(capacity - 1) * BRICK_SIZE] = BRICK_NULL;
	
}
