#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

out gl_PerVertex { vec4 gl_Position; float gl_PointSize;};
out vec4 WorldNormal;
out vec4 fragPosition;

uniform mat4 ModelMatrix; // local->world matrix]
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix; // local->NDC matrix

void main()
{
    WorldNormal = vec4(transpose(inverse(mat3(ModelMatrix)))  * vNormal, 0);
    fragPosition =  ModelMatrix * vec4(vPosition, 1);
    gl_Position = ProjectionMatrix * ViewMatrix * fragPosition;
}
