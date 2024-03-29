
#version 460 core

#include "shader/lib/blend.glsl"
#include "shader/lib/volume.glsl"

#define BLEND_MODE_NORMAL	1
#define BLEND_MODE_ERASE	2
#define BLEND_MODE_INSIDE	3
#define BLEND_MODE_COLOR	4

uniform float	opacity;
uniform int		blendMode;
uniform ivec3	nbrGroups;

shared bool oldBrickWasCopied;

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 localPos = ivec3(gl_LocalInvocationID);
	int localOffset = localPos.x + localPos.y * 8 + localPos.z * 8 * 8;

	for (int i = 0; i < nbrGroups.x; i++) {
		for (int j = 0; j < nbrGroups.y; j++) {
			for (int k = 0; k < nbrGroups.z; k++) {
				uint brickPos = i + j * nbrGroups.x + k * nbrGroups.x * nbrGroups.y;

				if (gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0){
					if (volume_2.bricks[brickPos] == BRICK_NULL) {
						volume_2.bricks[brickPos] = volume_1.bricks[brickPos];
						brickPool_increaseRefCount(volume_2.bricks[brickPos]);
						oldBrickWasCopied = true;			
					}
					else
						oldBrickWasCopied = false;
				}

				memoryBarrierShared();
				memoryBarrier();
				barrier();
	
				if (oldBrickWasCopied)
					continue;

				uint ptrVoxel_dst = volume_2.bricks[brickPos] * BRICK_SIZE + localOffset;

				ivec3 posVoxel = ivec3(i, j, k) * 8 + localPos;
				if (any(greaterThanEqual(posVoxel, getVolumeSize())))
					continue;
				
				vec4 a = unpackUnorm4x8(brickPool.data[ptrVoxel_dst]);

				vec4 b;
				uint brick_src = volume_1.bricks[brickPos];
				if (brick_src == BRICK_NULL)
					b = vec4(0,0,0,0);
				else {
					uint ptrVoxel_src = brick_src * BRICK_SIZE + localOffset;
					b= unpackUnorm4x8(brickPool.data[ptrVoxel_src]);
				}

				a.a *= opacity;
				vec4 c;
				switch (blendMode) {
				case BLEND_MODE_NORMAL:
					c = blend_over(a, b);
					break;
				case BLEND_MODE_ERASE:
					c = b;
					c.a -= opacity * a.a;
					break;
				case BLEND_MODE_INSIDE:
					if (b.a == 0)
						c = vec4(0);
					else
						c = blend_over(a, b);
					break;
				case BLEND_MODE_COLOR:
					if (b.a == 0)
						c = vec4(0);
					else {
						c = blend_over(a, b);
						c.a = b.a;
					}
					break;
				}


				uint brick = volume_2.bricks[brickPos];
				brickPool.data[ptrVoxel_dst] = packUnorm4x8(c);

			}
		}
	}
}


