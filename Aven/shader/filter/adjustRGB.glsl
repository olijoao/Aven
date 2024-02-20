#version 460 core

#define FILTER_COLOR
#include "shader/lib/filter_main.glsl"

uniform ivec3 offset;

vec3 filter_color(in vec3 color) {
    return color + vec3(offset)/256;
}