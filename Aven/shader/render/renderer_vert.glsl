
#version 450
layout(location = 0) in vec3 vertPos;

out vec2 pos_Pixel;

void main() {
	pos_Pixel = vertPos.xy;
	pos_Pixel.y *= -1;
	gl_Position = vec4(vertPos, 1.0);
	if (gl_Position.w != 0)
		gl_Position /= gl_Position.w;
}