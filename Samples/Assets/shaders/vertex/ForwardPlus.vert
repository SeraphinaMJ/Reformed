#version 430 core

// ModelMatrix, ViewMatrix, ProjectionMatrix
// vertex shader inputs
#include "material_dependencies.glsl" 

out VSOut
{
    vec2 UV;           // for material texture lookups
    vec4 WorldNormal;  // world space lighting
    vec4 fragPosition; // world space lighting
    vec4 clipPos;      // getting screen-space UV
} vsOut;


void main()
{
    vsOut.UV           = vUV;
    vsOut.WorldNormal  = vec4(transpose(inverse(mat3(ModelMatrix)))  * vNormal, 0);
    vsOut.fragPosition = ModelMatrix * vec4(vPosition, 1);
    vsOut.clipPos      = ProjectionMatrix * ViewMatrix * vsOut.fragPosition;

    gl_Position = vsOut.clipPos;
}
