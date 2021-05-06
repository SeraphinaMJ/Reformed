#version 430 core


#include "material_dependencies.glsl"

out vec4 WorldNormal;

out vec2 UV;
out vec4 fragPosition;

out gl_PerVertex { vec4 gl_Position; float gl_PointSize;};


void main()
{
  WorldNormal = transpose(inverse(ModelMatrix))  * vec4(vNormal, 0);

  fragPosition =  ModelMatrix *vec4(vPosition, 1); // in worldSpace for ligting

  // vec4 t = ModelMatrix * vec4(vTangent,0);
  // vec4 b = ModelMatrix * vec4(vBitangent,0);
  // vec4 n = ModelMatrix * vec4(vNormal,0);
  
 
  // TBN = mat4(t, b, n, vec4(0,0,0,1));

  //fragPosition = ModelMatrix * vec4(vPosition, 1);
  UV = vUV;
  
  gl_Position =  ProjectionMatrix * ViewMatrix * fragPosition;
}
