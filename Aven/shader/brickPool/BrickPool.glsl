#ifndef BRICKPOOL_GLSL
#define BRICKPOOL_GLSL

const uint	BRICK_NULL = 1 << 31;
const uint	BRICK_SIZE = 8 * 8 * 8;


layout(std430, binding = 0) buffer BrickPoolBuffer {
	uint countFree;		//index following last node, CPU expects this to be at index 0 to query nbr of free nodes
	uint capacity;
	uint ptr_nextFree;
	uint padding_0;
	uint data[];
} brickPool;



// returns BRICK_NULL if capacity == 0
// !! not thread safe -> only to be used by one thread across all workgroups
uint brickPool_alloc() {
	if (brickPool.countFree == 0)
		return BRICK_NULL;

	uint result = brickPool.ptr_nextFree;
	brickPool.ptr_nextFree = brickPool.data[brickPool.ptr_nextFree * BRICK_SIZE]; 
	brickPool.countFree--;
	return result;
}


// !! not thread safe -> only to be used by one thread across all workgroups, 
void brickPool_free(uint ptr) {
	if (ptr == BRICK_NULL)
		return;
	brickPool.data[ptr * BRICK_SIZE] = brickPool.ptr_nextFree;
	brickPool.ptr_nextFree = ptr;
	brickPool.countFree++;
}


#endif