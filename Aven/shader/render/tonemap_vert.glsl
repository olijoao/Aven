
#version 450
layout(location = 0) in vec3 vertPos;

out vec3 pos;

void main() {
	pos = vertPos*0.5f+0.5f;
	gl_Position = vec4(vertPos, 1.0);
	if (gl_Position.w != 0)
		gl_Position /= gl_Position.w;
}