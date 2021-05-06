#version 430 core

#include "inverse_proj.glsl"

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vUV;
out vec2 UVworld;
out vec2 UV;

uniform mat4 ProjectionMatrix;
out flat mat4 invProj;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
};


void main()
{
    invProj = invProjCalc(ProjectionMatrix);
    UV = 0.5 * ((vPosition.xy) + 1);
	UVworld = vUV;
    // [-1,1] -> [0,2] -> [0,1]
   
    gl_Position =  vec4(vPosition.xy,1,1);
}
