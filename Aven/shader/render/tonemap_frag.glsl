#version 450 core

in vec3 pos;
out vec4 FragColor;

uniform sampler2D	tex;
uniform float		gamma;



void main() {
	vec3 color = texture(tex, pos.xy).rgb;

	// gamam corection
    color = pow(color, 1.0f / vec3(gamma, gamma, gamma));
   
	FragColor = vec4(color.rgb, 1);
}

