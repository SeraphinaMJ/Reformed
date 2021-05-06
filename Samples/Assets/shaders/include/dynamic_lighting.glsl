// in order to include this header, you must define a constant LIGHT_COUNT
// which is the number of lights you would like to process at a time

#include "light.glsl"
#include "camera.glsl"

float calcAtten(in Light light, in vec3 fragPosition)
{
    const float dist = length(light.position.xyz - fragPosition);
    const float denominator =  
        light.disAtten.x +
        light.disAtten.y * dist +
        light.disAtten.z * dist * dist;

    return min(1/denominator, 1);
}

float calcSpotEffect(in Light light, in vec3 L)
{
    const vec3 lightDir = normalize(light.direction.xyz);
    const float alpha = max(dot(lightDir , -L),0);
    const float r =        (alpha -  cos(light.phi)) /
           (cos(light.theta) - cos(light.phi)) ;
    return clamp(pow(r, light.falloff), 0, 1);
}

bool hasDecay(in Light light)
{
    return light.type != DIRECTIONAL && light.ifDecay;
}

vec3 calcIDiffuse(in Light light, in vec3 normal, in vec3 L)
{
    return light.diffuse.rgb * max(dot(normal,L), 0.3);
}

vec3 calcISpecular(
    in Light light,
    in vec3 normal, in vec3 L,
    in vec3 fragPosition,
    in float specularExp)
{
    const vec3 R = normalize(2 * dot(normal,L)*normal - L);
    const vec3 V = normalize(camera.position.xyz - fragPosition);
    return light.specular.rgb * pow(clamp(dot(R,V), 0.001, 1.0), 2*specularExp);
}

// returns diffuse, specular, and ambient colors pre-shadowing
mat3 CalcILocal(in Light light, in vec3 fragPosition, in vec3 normal, in float spec)
{    
    const vec3 L = normalize(
        light.type == DIRECTIONAL ?
            -light.direction.xyz  : 
            light.position.xyz - fragPosition
    );

    const vec3 Idiffuse = calcIDiffuse(light, normal, L);
    const vec3 Ispecular = calcISpecular(light, normal, L, fragPosition, spec);

    const float attFactor = hasDecay(light) ?
        calcAtten(light, fragPosition)      :
        1;

    const float spotlightEffect = light.type == SPOT ?
        calcSpotEffect(light, L)                     :
        1;

    const float factors  = light.intensity * attFactor;
    const float factors2 = factors * spotlightEffect;
    
    return mat3(
        factors2 * Idiffuse,
        factors2 * Ispecular,
        factors * light.ambient.rgb
    );
}