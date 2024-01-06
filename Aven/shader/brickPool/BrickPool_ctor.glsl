#version 460 core

#pragma include "shader/brickPool/BrickPool.glsl"

uniform unsigned int capacity;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	brickPool.countFree	= capacity;
	brickPool.capacity	= capacity;
	brickPool.offset_refCount = capacity * BRICK_SIZE;
	brickPool.ptr_nextFree = 0;

	// set ptr to next brick
	for (int i = 0; i < capacity-1; i++)
		brickPool.data[i*BRICK_SIZE] = i+1;
	brickPool.data[(capacity - 1) * BRICK_SIZE] = BRICK_NULL;

	//set reference counts = 0
	for (int i = 0; i < capacity; i++)
		brickPool.data[brickPool.offset_refCount +i] = 0;
	
}
