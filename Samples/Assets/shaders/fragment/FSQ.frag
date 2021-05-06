#version 430 core

in vec2 UV;

layout(binding=0) uniform sampler2D tex;


layout(location=0) out vec4 vFragColor;

uniform uvec4 channelmapper;
uniform vec4 premultiply;
uniform vec4 postadd;

void main()
{
    const vec4 sampled = premultiply * texture(tex, UV) + postadd;

    for(uint i = 0; i != 4; ++i)
    {
        vFragColor[i] = sampled[channelmapper[i]];
    }
}

