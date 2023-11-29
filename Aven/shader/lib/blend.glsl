
#ifndef BLEND_GLSL
#define BLEND_GLSL



// a over b
vec4 blend_over(vec4 a, vec4 b) {
	float alpha = a.a + b.a * (1.0f - a.a);
	vec3 color = (a.rgb * a.a + b.rgb * b.a * (1.0f - a.a)) / alpha;
	return vec4(color, alpha);
}


#endif	//BLEND_GLSL
