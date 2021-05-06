#version 430 core


layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;
layout(location = 4) in vec3 vBitangent;

out vec4 WorldNormal;
out vec3 uvNormal;
out vec2 UV;
out vec4 fragPosition;

out gl_PerVertex { vec4 gl_Position; float gl_PointSize;};


uniform mat4 ModelMatrix; // local->world matrix
uniform mat4 ViewProjectionMatrix; // local->NDC matrix


void main()
{
  WorldNormal = normalize(ModelMatrix*vec4(vNormal, 0));

  fragPosition = ModelMatrix * vec4(vPosition, 1);

  // vec4 t = ModelMatrix * vec4(vTangent,0);
  // vec4 b = ModelMatrix * vec4(vBitangent,0);
  // vec4 n = ModelMatrix * vec4(vNormal,0);
  
 
  // TBN = mat4(t, b, n, vec4(0,0,0,1));

  //fragPosition = ModelMatrix * vec4(vPosition, 1);
  uvNormal = vPosition;
  UV = vUV;

 mat4 modelViewMatrix = ViewProjectionMatrix *  ModelMatrix;

  gl_Position =  modelViewMatrix * vec4(vPosition.xyz, 1);

}
