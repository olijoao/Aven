
#ifndef SAMPLE_GLSL
#define SAMPLE_GLSL

#pragma include "shader/lib/math.glsl"

// rand
float rand_internal;

void srand(float seed) {
    rand_internal = seed; 
}

float rand() {
    return fract(sin(rand_internal += 0.1f) * 43758.5453123);
}


vec3 randomSpherePoint() {
    float ang1 = rand() * 2 * PI;
    float u = rand() * 2 - 1;
    float sqrt1MinusU2 = sqrt(1.0 - u * u);
    float x = sqrt1MinusU2 * cos(ang1);
    float y = sqrt1MinusU2 * sin(ang1);
    float z = u;
    return vec3(x, y, z);
}

vec3 randomHemispherePoint(vec3 n) {
    vec3 v = randomSpherePoint();
    return v * sign(dot(v, n));
}




#endif	//SAMPLE_GLSL