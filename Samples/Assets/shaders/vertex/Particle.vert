#version 430 core

layout(location = 0) in vec4 vPosition;
//layout(location = 1) in vec2 aTexCoord;
layout(location = 1) in vec4 pPosition;
layout(location = 2) in vec4 vColor;

out vec4 color;
//out vec2 TexCoord;

uniform mat4 ModelMatrix; // local->world matrix]
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix; // local->NDC matrix


void main(){

vec3 CameraRight_worldspace = {ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]};
vec3 CameraUp_worldspace = {ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]};

vec3 vertexPosition_worldspace =
    pPosition.xyz
    + CameraRight_worldspace * vPosition.x * pPosition.w
    + CameraUp_worldspace * vPosition.y * pPosition.w;

	// // remove rotation
	 mat4 modelViewMatrix =  ViewMatrix *  ModelMatrix;

	// // First colunm.
	// modelViewMatrix[0][0] = 1.0; 
	// modelViewMatrix[0][1] = 0.0; 
	// modelViewMatrix[0][2] = 0.0; 

	// // Second colunm.
	// modelViewMatrix[1][0] = 0.0; 
	// modelViewMatrix[1][1] = 1.0; 
	// modelViewMatrix[1][2] = 0.0; 


	// // Thrid colunm.
	// modelViewMatrix[2][0] = 0.0; 
	// modelViewMatrix[2][1] = 0.0; 
	// modelViewMatrix[2][2] = 1.0; 

	gl_Position = ProjectionMatrix * ViewMatrix * vec4(vertexPosition_worldspace,1);
	//TexCoord = aTexCoord;
	//gl_Position =  ProjectionMatrix * modelViewMatrix * vec4(vPosition.xyz, 1);
	color = vColor;
}

