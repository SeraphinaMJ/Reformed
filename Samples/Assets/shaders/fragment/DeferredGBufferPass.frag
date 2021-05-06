#version 430 core

in vec3 WorldNormal;
//in vec2 UV;

layout(location = 0) out vec4 g_worldNormal;

//layout(binding=2)uniform sampler2D normalTexture;

uniform struct
{
  float specularExponent; // shininess

  //bool hasNormalMapTexture
}material;

void normalMap()
{
}

void main()
{
    g_worldNormal = vec4(normalize(WorldNormal), material.specularExponent);
    // TODO: normal mapping?
}
