#version 460 core

#pragma include "shader/lib/volume.glsl"

uniform uint nbrBricks;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() {
	for (int i = 0; i < nbrBricks; i++)
		brickPool_free(volume_2.bricks[i]);
}



