#version 430 core

#include "material.glsl" // material struct, material getters
#include "dynamic_lighting.glsl"  // camera, light, dyanmic lighting functions

#ifndef LIGHT_COUNT
// default number of shadow casters
#define LIGHT_COUNT 4
#endif

uniform Light Lights[LIGHT_COUNT];
uniform int lightCount;

/**//**//**/
layout(location = 0) out vec4 vFragColor;

in VSOut
{
    vec2 UV;           // for material texture lookups
    vec4 WorldNormal;  // world space lighting
    vec4 fragPosition; // world space lighting
    vec4 clipPos;      // getting screen-space UV
}fsIn;

layout(binding = 0) uniform sampler2D depth;

layout(binding = 1) uniform sampler2D diffuseTexture;
layout(binding = 2) uniform sampler2D specularTexture;

layout(binding = 3) uniform sampler2D lightAcc;
layout(binding = 4) uniform sampler2D specAcc;
layout(binding = 5) uniform sampler2D ambAcc;
uniform vec4 globalAmbient;

layout(binding = 6) uniform sampler2D gNormal;

layout(binding = 7) uniform sampler2D shadowMap;

mat3 getShadowLighting(in vec2 UVscreen)
{
    mat3 Icolor = mat3(0);
    const vec4 inLight = texture(shadowMap, UVscreen);
    const vec4 sampled = texture(gNormal, UVscreen);
    
    const vec3 normal = normalize(sampled.xyz);
    const float spec = sampled.w;
    
    for(int i = 0, end = min(lightCount, LIGHT_COUNT); i < end; ++i)
    {
        mat3 lightColor = CalcILocal(Lights[i], fsIn.fragPosition.xyz, normal, spec);

        // diffuse shows a bit in shadow
        lightColor[0] *= max(0.3, inLight[i]);
        
        // specular does not show at all in shadow
        lightColor[1] *= inLight[i];
        
        Icolor += lightColor;
    }
    
    return (Icolor);
}

void main()
{
    const vec2 UVscreen = ((fsIn.clipPos.xy / fsIn.clipPos.w) + 1) / 2;

    // material coefficients
    const vec4 kD = mat_diffuse(diffuseTexture, fsIn.UV);
    const vec4 kS = mat_specular(specularTexture, fsIn.UV);
    const vec4 kA = mat_ambient(kD);
        
    const mat3 shadowLighting = getShadowLighting(UVscreen);
    const vec3 Idiffuse =  kD.rgb * (shadowLighting[0] + texture(lightAcc, UVscreen).rgb);
    const vec3 Ispecular = kS.rgb * (shadowLighting[1] + texture(specAcc, UVscreen).rgb);
    const vec3 Iambient =  kA.rgb * (shadowLighting[2] + (texture(ambAcc, UVscreen).rgb + globalAmbient.rgb));
    const vec3 color = Idiffuse + Ispecular + Iambient + material.emissiveColor.rgb;

    const vec4 Ifinal = vec4(camera_fog(color, fsIn.fragPosition), 1);
    vFragColor = Ifinal;//Ifinal;//(normal + 1)/2;
}
