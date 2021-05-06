#version 430 core

in vec4 WorldNormal;
in vec4 fragPosition;

#include "camera.glsl"

uniform vec4 color;

layout(location = 0) out vec4 vFragColor;

void main()
{
    vec4 N = normalize(WorldNormal);
    vec4 L = normalize(camera.position - fragPosition);
    
    vFragColor = vec4(abs(dot(N, L)) * color.rgb, color.a);
}
