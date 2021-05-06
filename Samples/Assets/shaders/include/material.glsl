uniform struct
{
  bool receiveLight;
  
  vec4 ambientColor; // how much ambient light to absorb
  vec4 diffuseColor; // how much diffuse light to absorb
  vec4 emissiveColor;
  vec4 specularColor;
  float specularExponent; // shininess  
  
  bool hasDiffuseTexture;
  bool hasSpecularTexture;
  bool hasNormalMapTexture;

}material;

uniform bool hasTexture;

// Material coefficient terms
vec4 mat_diffuse(in sampler2D diffuseTexture, in vec2 UV)
{
    return material.hasDiffuseTexture ?
        texture(diffuseTexture, UV)   :
        material.diffuseColor;
}

vec4 mat_specular(in sampler2D specularTexture, in vec2 UV)
{
    return material.hasSpecularTexture ?
        texture(specularTexture, UV)   :
        material.specularColor;
}

vec4 mat_ambient(in vec4 diffuseColor)
{
    return material.hasDiffuseTexture ?
        diffuseColor                  :
        material.ambientColor;
}
