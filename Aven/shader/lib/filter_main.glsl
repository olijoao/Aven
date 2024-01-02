#ifndef FILTER_MAIN_GLSL
#define FILTER_MAIN_GLSL

#pragma include "shader/lib/volume.glsl"


//forward declaration
vec4 getResult(ivec3 pos);	


uniform ivec3 nbrGroups;


layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 localPos = ivec3(gl_LocalInvocationID);
	int localOffset = localPos.x + localPos.y * 8 + localPos.z * 8 * 8;
	
	for (int i = 0; i < nbrGroups.x; i++) {
		for (int j = 0; j < nbrGroups.y; j++) {
			for (int k = 0; k < nbrGroups.z; k++) {
				uint brickPos = i + j * nbrGroups.x + k * nbrGroups.x * nbrGroups.y;

				if (gl_LocalInvocationID.x == 0 && gl_LocalInvocationID.y == 0 && gl_LocalInvocationID.z == 0) {
					volume_2.bricks[brickPos] = brickPool_alloc();
				}

				memoryBarrier();
				barrier();

				ivec3 posVoxel = ivec3(i, j, k) * 8 + localPos;
				if (any(greaterThanEqual(posVoxel, getVolumeSize())))
					continue;

				uint brick = volume_2.bricks[brickPos];
				vec4 result = getResult(posVoxel);
				brickPool.data[brick * BRICK_SIZE + localOffset] = packUnorm4x8(result);
			}
		}
	}
}



// FILTER_COLOR: vec3 -> vec3
#if defined(FILTER_COLOR)			
vec3 filter_color(in vec3 color);	//forward declaration

vec4 getResult(ivec3 posVoxel) {
	vec4 voxel = getVolume_src(posVoxel);
	return vec4(filter_color(voxel.rgb), voxel.a);
}



// FILTER_COLOR_ALPHA: vec4 -> vec4
#elif defined(FILTER_COLOR_ALPHA)			
vec4 filter_color_alpha(in vec4 color);	//forward declaration

vec4 getResult(ivec3 posVoxel) {
	vec4 voxel = getVolume_src(posVoxel);
	return filter_color_alpha(voxel);
}



//FILTER_RENDER: pos(vec3) -> vec4
#elif defined(FILTER_RENDER)	
vec4 filter_render(ivec3 pos);			//forward declaration

vec4 getResult(ivec3 posVoxel) {
	return filter_render(posVoxel);
}



//FILTER_DEFORM: pos(vec3) -> pos(vec3)
#elif defined(FILTER_DEFORM)	
ivec3 filter_deform(in ivec3 pos);			//forward declaration

vec4 getResult(ivec3 posVoxel) {
	return getVolume_src(filter_deform(posVoxel));
}


#else
	#error Included filter_main.glsl without defining type of filter.

#endif





#endif //FILTER_MAIN_GLSL
