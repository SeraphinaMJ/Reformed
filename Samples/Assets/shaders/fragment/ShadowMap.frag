#version 430 core

#include "transform.glsl"

uniform mat4 LightMatrix[4];
uniform vec3 LightDir[4];
uniform int lightCount;

in vec2 UV;
in flat mat4 invProj;

layout(binding = 0) uniform sampler2DShadow  shadowAtlas;
layout(binding = 1) uniform sampler2D  depth;
layout(binding = 2) uniform sampler2D worldNormal;

layout(location = 0) out vec4 shadow;

// accumulate shadow values across a kernel, then average them
float PCF(in vec3 projPos, in int light)
{
    const float kernelSize = 3;
    
    if(projPos.z > 1.0)
    {
        return 1.0;
    }

    // check whether current frag pos is in shadow
    #define SHADOW_CALCULATION(i, j) \
        textureOffset(shadowAtlas, projPos.xyz, ivec2(i, j))

    return (
        // sum result over an n * n grid
        #pragma TempestKernelLooping SHADOW_CALCULATION 3
    ) / (kernelSize * kernelSize);
}

void main()
{
    const vec3 worldPos = calcPositionFromDepth(invProj, texture(depth, UV).x, UV);
    vec4 inLight = vec4(1.,1.,1.,1.);
    for(int i = 0; i < lightCount; ++i)
    {
        const vec4 projPos = (LightMatrix[i] * vec4(worldPos, 1.));
        // [-w,w] -> [-1,1] -> [-1/2, 1/2] -> [0,1]
        const vec3 lightPos = 0.5 * (projPos.xyz / projPos.w) + 0.5;
        const vec2 shadowUV = vec2((lightPos.x + i)/4, lightPos.y);
        const vec3 atlasPos = vec3(shadowUV, lightPos.z);
        inLight[i] = PCF(atlasPos, i);
        //inLight[i] = atlasPos.z;
        //inLight = vec4(atlasPos , 1);
    }
    shadow = inLight;
}
