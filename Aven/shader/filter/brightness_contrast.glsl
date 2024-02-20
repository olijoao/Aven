#version 460 core

#define FILTER_COLOR
#include "shader/lib/filter_main.glsl"

uniform float contrast;
uniform float brightness;

vec3 filter_color(in vec3 color) {
    return (color - 0.5f) * contrast + 0.5f + brightness;
}