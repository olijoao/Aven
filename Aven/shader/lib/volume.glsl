#ifndef VOLUME_GLSL
#define VOLUME_GLSL

#include "shader/brickPool/BrickPool.glsl"


layout(std430, binding = 1) coherent buffer Volume_1 {
	uint bricks[];
} volume_1;

layout(std430, binding = 2) coherent buffer Volume_2 {
	uint bricks[];
} volume_2;

uniform ivec3 volume_size;


ivec3 getVolumeSize() {
	return volume_size;
}


vec4 getVolume_src(ivec3 pos) {
	if (any(greaterThanEqual(pos, volume_size))) 
		return vec4(0,0,0,0);

	ivec3 nbrBricks = (volume_size + 7) / 8;
	ivec3	brick_pos	= pos / 8;
	uint	brick_index = brick_pos.x 
						+ brick_pos.y * nbrBricks.x
						+ brick_pos.z * nbrBricks.x * nbrBricks.y;
	uint brick = volume_1.bricks[brick_index];
	if (brick == BRICK_NULL)
		return vec4(0,0,0,0);

	ivec3 local_pos = pos - brick_pos*8;
	uint local_offset = local_pos.x + local_pos.y * 8 + local_pos.z * 8 * 8;
	uint data = brickPool.data[brick * BRICK_SIZE + local_offset];

	return unpackUnorm4x8(data);
}


vec4 getVolume_dst(ivec3 pos) {
	if (any(greaterThanEqual(pos, volume_size)))
		return vec4(0, 0, 0, 0);	

	ivec3 nbrBricks = (volume_size + 7) / 8;
	ivec3	brick_pos	= pos / 8;
	uint	brick_index = brick_pos.x 
						+ brick_pos.y * nbrBricks.x
						+ brick_pos.z * nbrBricks.x * nbrBricks.y;
	uint brick = volume_2.bricks[brick_index];

	if (brick == BRICK_NULL)
		return vec4(0, 0, 0, 0);

	ivec3 local_pos = pos - brick_pos*8;
	uint local_offset = local_pos.x + local_pos.y * 8 + local_pos.z * 8 * 8;
	uint data = brickPool.data[brick * BRICK_SIZE + local_offset];
	return unpackUnorm4x8(data);
}



#endif // VOLUME_GLSL