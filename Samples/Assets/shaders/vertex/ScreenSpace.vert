#version 430 core

// ScreenSpace Shader

layout(location = 0) in vec3 vPosition; 
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

uniform mat4 ModelMatrix; // local->world matrix
uniform mat4 OrthoMatrix;
uniform mat4 ViewMatrix;

uniform vec3 Scale;
uniform vec3 Transform;

uniform float CameraWidth;
uniform float CameraHeight;


out vec2 uv;




void main(){

  float newX = ((vPosition.x * (Scale.x/ (CameraWidth)))+ Transform.x/CameraWidth);
  float newY = ((vPosition.y * (Scale.y/ (CameraHeight))) + Transform.y/CameraHeight);

	//vec2 newuv = vec2((vPosition.x) * .5f -.5f, (vPosition.xy) * .5f - .5f	);
	uv = (vPosition.xy + 1) * .5f ;

	//gl_Position =  vec4(vPosition.xy, 0, 1);
	gl_Position =  vec4(newX, newY, 0, 1);
}

