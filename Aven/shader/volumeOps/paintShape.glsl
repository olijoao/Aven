
#version 460 core
#pragma include "shader/lib/blend.glsl"

uniform layout(rgba8, binding = 1) image3D volume;

uniform ivec3	aabb_intersection_min;
uniform ivec3	aabb_intersection_max;
uniform vec4	color;
uniform ivec3	pos_center;
uniform int		radius;
uniform int		shape;

#define SHAPE_SPHERE	0
#define SHAPE_CUBE		1
#define SHAPE_CONE		2
#define SHAPE_CYLINDER	3

layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
void main() {
	ivec3 posVoxel = aabb_intersection_min + ivec3(gl_GlobalInvocationID);

	if (any(greaterThan(posVoxel, aabb_intersection_max)))
		return;

	switch(shape) {
	case SHAPE_SPHERE:
		if (distance(pos_center, posVoxel) > radius)
			return;
		break;
	case SHAPE_CUBE:
		break;
	case SHAPE_CONE:
		float height_01 = distance(posVoxel.y, pos_center.y+radius)/(float(radius+radius));
		if (distance(posVoxel.xz, pos_center.xz)/float(radius) > height_01)
			return;
	case SHAPE_CYLINDER:
		if (distance(posVoxel.xz, pos_center.xz) > float(radius))
			return;
		break;
	}

	vec4 oldColor = imageLoad(volume, posVoxel);
	vec4 outputColor = blend_over(color, oldColor);
	imageStore(volume, posVoxel, outputColor);
}
