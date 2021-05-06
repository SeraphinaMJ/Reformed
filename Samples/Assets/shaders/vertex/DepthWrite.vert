#version 430 core

layout(location = 0) in vec3 vPosition;

out gl_PerVertex { vec4 gl_Position; };

uniform mat4 ModelMatrix; // local->world matrix]
uniform mat4 LightMatrix;  // world->lightSpace matrix

void main()
{
    gl_Position =  LightMatrix * ModelMatrix * vec4(vPosition.xyz, 1);
}
