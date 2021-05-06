#version 430 core

in vec3 uvNormal;

layout(binding=3) uniform samplerCube cubeTexture;

layout(location = 0) out vec4 vFragColor;

void main()
{
    vFragColor = texture(cubeTexture, uvNormal); 
}
