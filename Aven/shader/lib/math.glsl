#ifndef MATH_GLSL
#define MATH_GLSL


#define PI			3.14159265358
#define PI2			6.28318530718
#define PI4			12.5663706143
#define HALF_PI		1.57079632679
#define QUARTER_PI	0.78539816339
#define INV_PI		0.31830988618
#define HALF_INV_PI	0.15915494309

#define FLOAT_MAX	3.402823466e+38


float distanceSquared(vec3 A, vec3 B) {
    vec3 C = A - B;
    return dot( C, C );
}

#endif	//MATH_GLSL
