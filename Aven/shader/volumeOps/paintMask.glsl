
#version 460 core

#pragma include "shader/lib/blend.glsl"

#define BLEND_MODE_NORMAL	0
#define BLEND_MODE_ERASE	1


readonly	uniform layout(rgba8, binding = 0)	image3D volume_src;
readonly	uniform layout(r8,	binding = 1)	image3D volume_mask;
writeonly	uniform layout(rgba8, binding = 2)	image3D volume_dst;

uniform ivec3	volume_size;
uniform float	opacity;
uniform vec3	color;
uniform int		blendMode;

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = ivec3(gl_GlobalInvocationID);

	if (any(greaterThanEqual(posVoxel, volume_size)))
		return;

	vec4 color_src	= imageLoad(volume_src, posVoxel);
	float mask		= imageLoad(volume_mask, posVoxel).r;

	vec4 c;
	switch (blendMode) {
	case BLEND_MODE_NORMAL:
		c = blend_over(vec4(color, opacity * mask), color_src);
		break;
	case BLEND_MODE_ERASE:
		c = color_src;
		c.a -= opacity*mask;
		break;
	}
	
	imageStore(volume_dst, posVoxel, c);
}
