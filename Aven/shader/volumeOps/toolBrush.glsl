
#version 460 core
#include "shader/lib/blend.glsl"
#include "shader/lib/volume.glsl"

uniform layout(rgba8, binding = 1) image3D volume;

uniform ivec3	aabb_intersection_min;
uniform ivec3	aabb_intersection_max;
uniform vec4	color;
uniform ivec3	pos_center;
uniform int		radius;
uniform int		shape;

uniform ivec3 posBrickMin;
uniform ivec3 posBrickMax;
uniform ivec3 nbrGroupsInVolume;

#define SHAPE_SPHERE	0
#define SHAPE_CUBE		1
#define SHAPE_CONE		2
#define SHAPE_CYLINDER	3

shared bool brickWasNull;


float getBrushDensity(ivec3 posVoxel) {
	switch (shape) {
	case SHAPE_SPHERE:
		if (distance(pos_center, posVoxel) > radius)
			return 0;
		return 1;
	case SHAPE_CUBE:
		return 1;
	case SHAPE_CONE:
		float height_01 = distance(posVoxel.y, pos_center.y + radius) / (float(radius + radius));
		if (distance(posVoxel.xz, pos_center.xz) / float(radius) > height_01)
			return 0;
		return 1;
	case SHAPE_CYLINDER:
		if (distance(posVoxel.xz, pos_center.xz) > float(radius))
			return 0;
		return 1;
	}
	return 1;
}



layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 localPos = ivec3(gl_LocalInvocationID);
	int localOffset = localPos.x + localPos.y * 8 + localPos.z * 8 * 8;

	for (int i = posBrickMin.x; i <= posBrickMax.x; i++) {
		for (int j = posBrickMin.y; j <= posBrickMax.y; j++) {
			for (int k = posBrickMin.z; k <= posBrickMax.z; k++) {
				
				uint brickPos = i + j * nbrGroupsInVolume.x + k * nbrGroupsInVolume.x * nbrGroupsInVolume.y;

				if (gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0){
					if (volume_2.bricks[brickPos] == BRICK_NULL) {
						volume_2.bricks[brickPos] = brickPool_alloc();
						brickWasNull = true;
					}
					else
						brickWasNull = false;
				}

				memoryBarrierShared();
				memoryBarrier();
				barrier();

				vec4 paintColor = vec4(color);
				
				ivec3 posVoxel = ivec3(i, j, k) * 8 + ivec3(gl_LocalInvocationID);
				if (any(greaterThan(posVoxel, aabb_intersection_max))
					|| any(lessThan(posVoxel, aabb_intersection_min))) 
				{
					paintColor = vec4(0);
				}
					
				paintColor.a *= getBrushDensity(posVoxel);
				
				uint brick = volume_2.bricks[brickPos];
				vec4 oldColor;
				if (brickWasNull)
					oldColor = vec4(0);
				else
					oldColor = unpackUnorm4x8(brickPool.data[brick * BRICK_SIZE + localOffset]);

				paintColor = blend_over(paintColor, oldColor);
				brickPool.data[brick * BRICK_SIZE + localOffset] = packUnorm4x8(paintColor);
			}
		}
	}


}
