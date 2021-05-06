#version 430 core

#include "material.glsl"
#include "camera.glsl"
#include "light.glsl"
#include "dynamic_lighting.glsl"

layout(location = 0) out vec4 vFragColor;

// in vec4 WorldNormal;
in vec2 UV;
in vec4 fragPosition;
in vec4 WorldNormal;

//in vec4 WorldPosition;
//in vec2 Uv1;//fixed border line
//in mat4 TBN;
//in vec4 PhongLightingColor;

layout(binding=0)uniform sampler2D diffuseTexture;
layout(binding=1)uniform sampler2D specularTexture;
layout(binding=2)uniform sampler2D normalTexture;

layout(binding=3)uniform samplerCube cubeTexture;
layout(binding=4)uniform sampler2D simpleTexture;

// represents material properties of the surface passed by the application

#define LIGHT_COUNT 32 // maximum possible lights this shader supports
uniform Light Lights[LIGHT_COUNT]; // support UP TO 32 lights
uniform int lightCount; // # of lights enabled

uniform vec4 globalAmbient;
//uniform vec4 TintColor;
//uniform int ShadingModel;

mat3 accumulateLighting()
{
   const vec3 normal = normalize(WorldNormal).xyz;
   const int count = min(lightCount, LIGHT_COUNT);
   
   mat3 ILocal = mat3(0);
   
   for(int i = 0; i < count; ++i)
   {
      ILocal += CalcILocal(Lights[i], fragPosition.xyz, normal, material.specularExponent);
   }
   
   return ILocal;
}

void blinn_shading()
{    // material coefficients
    const vec4 kD = mat_diffuse(diffuseTexture, UV);
    const vec4 kS = mat_specular(specularTexture, UV);
    const vec4 kA = mat_ambient(kD);
    
    const mat3 ILocal = accumulateLighting();
    
    const vec3 color = (
      ILocal[0] * kD.rgb +                        // Diffuse
      ILocal[1] * kS.rgb +                        // Specular
      (ILocal[2] + globalAmbient.rgb) * kA.rgb +  // Ambient
      material.emissiveColor.rgb                  // Emissive
    );

    const float s = (camera.z_far - distance(camera.position, fragPosition)) /
        (camera.z_far - camera.z_near);

    
    const vec4 Ifinal = mix(camera.i_fog, vec4(color, 1.), s);

    vFragColor = Ifinal;
}


void main()
{

  //vec4 normal = WorldNormal;
  //vFragColor = normal;
     
  blinn_shading();
}
