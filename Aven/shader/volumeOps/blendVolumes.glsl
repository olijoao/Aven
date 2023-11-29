
#version 460 core

#pragma include "shader/lib/blend.glsl"

readonly  uniform layout(rgba8, binding = 0) image3D volume_src;
uniform layout(rgba8, binding = 1) image3D volume_dst;

uniform ivec3 volume_src_size;
uniform ivec3 volume_dst_size;
uniform float opacity;

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);

	if (any(greaterThanEqual(posVoxel, volume_dst_size)))
		return;

	vec4 a = imageLoad(volume_dst, posVoxel);
	vec4 b = imageLoad(volume_src, posVoxel);
	a.a *= opacity;
	vec4 c = blend_over(a,b);

	imageStore(volume_dst, posVoxel, c);
}
