#ifndef FILTER_MAIN_GLSL
#define FILTER_MAIN_GLSL

readonly  uniform layout(rgba8, binding = 0) image3D volume_src;
writeonly uniform layout(rgba8, binding = 1) image3D volume_dst;

uniform ivec3 volume_dst_size;


// FILTER_COLOR: vec3 -> vec3
#if defined(FILTER_COLOR)			
vec3 filter_color(in vec3 color);	//forward declaration

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);
	if (any(greaterThanEqual(posVoxel, volume_dst_size)))
		return;
	vec4 voxel = imageLoad(volume_src, posVoxel);
	voxel = vec4(filter_color(voxel.rgb), voxel.a);
	imageStore(volume_dst, posVoxel, voxel);
}



// FILTER_COLOR_ALPHA: vec4 -> vec4
#elif defined(FILTER_COLOR_ALPHA)			
vec4 filter_color_alpha(in vec4 color);	//forward declaration

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);
	if (any(greaterThanEqual(posVoxel, volume_dst_size)))
		return;
	vec4 voxel = imageLoad(volume_src, posVoxel);
	voxel = filter_color_alpha(voxel);
	imageStore(volume_dst, posVoxel, voxel);
}


//FILTER_RENDER: pos(vec3) -> vec4
#elif defined(FILTER_RENDER)	
vec4 filter_render(ivec3 pos);			//forward declaration

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);
	if (any(greaterThanEqual(posVoxel, volume_dst_size)))
		return;
	imageStore(volume_dst, posVoxel, filter_render(posVoxel));
}

//FILTER_DEFORM: pos(vec3) -> pos(vec3)
#elif defined(FILTER_DEFORM)	
ivec3 filter_deform(in ivec3 pos);			//forward declaration

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);
	if (any(greaterThanEqual(posVoxel, volume_dst_size)))
		return;
	vec4 voxel = imageLoad(volume_src, filter_deform(posVoxel));
	imageStore(volume_dst, posVoxel, voxel);
}


#else
	#error Included filter_main.glsl without defining type of filter.

#endif





#endif //FILTER_MAIN_GLSL
