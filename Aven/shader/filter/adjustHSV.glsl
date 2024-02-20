#version 460 core

#define FILTER_COLOR
#include "shader/lib/filter_main.glsl"

uniform int offset_hue;
uniform int offset_saturation;
uniform int offset_value;


// c in [0,1]
vec3 rgb2hsv(vec3 c){
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}


// c in [0,1]
vec3 hsv2rgb(vec3 c){
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 filter_color(in vec3 color) {
    vec3 hsv = rgb2hsv(color);
    hsv.x = fract(hsv.x + float(offset_hue)/255.0f);
    hsv.y = clamp(hsv.y + float(offset_saturation)/100.0f, 0,1);
    hsv.z = clamp(hsv.z + float(offset_value)/100.0f,0,1);
    return hsv2rgb(hsv);
}