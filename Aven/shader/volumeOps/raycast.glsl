
#version 460 core

readonly uniform layout(rgba8, binding = 0) image3D volume;
uniform ivec3	volume_size;
uniform vec3	ray_pos;
uniform vec3	ray_dir;


layout(std430, binding = 6) writeonly buffer Result {
	ivec3 pos;
	int padding_0;
}	result;


// based on "A Fast Voxel Traversal Algorithm for Ray Tracing" by John Amanatides and Andrew Woo
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	vec3 pos	= ray_pos;
	ivec3 step	= ivec3(sign(ray_dir));

	//Value of t at which the ray crosses the first vertical voxel boundary and store it in variable tMaxX.

	ivec3 boundary;
	boundary.x = int((step.x > 0)?pos.x+1:pos.x);
	boundary.y = int((step.y > 0)?pos.y+1:pos.y);
	boundary.z = int((step.z > 0)?pos.z+1:pos.z);

	vec3 tMax = (vec3(boundary) - ray_pos) / ray_dir;
		
	//tDelta.x indicates how far along the ray we must move	(in units of t) for the horizontal component of such a movement to equal the width of a voxel.
	vec3 tDelta = 1.0f/ray_dir * step; 

	int nbrIterations = 0;

	while(true) {
		if (tMax.x < tMax.y) {
			if (tMax.x < tMax.z) {
				pos.x += step.x;
				if (pos.x >= volume_size.x || pos.x < 0)
					break;
				tMax.x += tDelta.x;
			}
			else {
				pos.z += step.z;
				if (pos.z >= volume_size.z || pos.z < 0)
					break;
				tMax.z += tDelta.z;
			}
		}
		else {
			if (tMax.y < tMax.z) {
				pos.y = pos.y + step.y;
				if (pos.y >= volume_size.y || pos.y < 0)
					break;
				tMax.y += tDelta.y;
			}
			else {
				pos.z += step.z;
				if (pos.z >= volume_size.z || pos.z < 0)
					break;
				tMax.z += tDelta.z;
			}
		}

		vec4 voxel = imageLoad(volume, ivec3(pos));
		if (voxel.a > 0)
			break;
	}

	result.pos = ivec3(pos);


}





