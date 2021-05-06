#version 430 core

layout(location = 0) out vec4 vDiffuseColor;
layout(location = 1) out vec3 vSpecColor;
layout(location = 2) out vec3 vAmbColor;

layout(binding = 0) uniform sampler2D gNormal;
layout(binding = 1) uniform sampler2D depth;

#include "camera.glsl"
#include "light.glsl"
#include "transform.glsl"
#include "dynamic_lighting.glsl"

uniform Light Lights[1];

in vec2 UV;
in flat mat4 invProj;

void main()
{
    const vec4 sampled = texture(gNormal, UV);

    const mat3 color = CalcILocal(
        Lights[0],
        calcPositionFromDepth(invProj, texture(depth, UV).r, UV),
        normalize(sampled.xyz),
        sampled.w
     );

    vDiffuseColor.rgb = color[0];
    vSpecColor        = color[1];
    vAmbColor         = color[2];
}
