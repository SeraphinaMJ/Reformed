#version 430 core

in vec4 WorldNormal;
in vec4 fragPosition;
in flat vec4 color;

uniform struct
{
   float fogAmmount;
  vec4 position;
  vec4 i_fog;
  float z_near;
  float z_far;
}camera;

layout(location = 0) out vec4 vFragColor;

void main()
{
    vec4 N = normalize(WorldNormal);
    vec4 L = normalize(camera.position - fragPosition);
    
    vFragColor = vec4(abs(dot(N, L)) * color.rgb, color.a);
}
