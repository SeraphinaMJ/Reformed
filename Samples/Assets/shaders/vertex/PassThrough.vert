#version 430 core

layout(location = 0) in vec3 vertexPosition_modelspace;



uniform mat4 ModelMatrix; // local->world matrix
uniform mat4 ViewProjectionMatrix; // local->NDC matrix

uniform bool Deffered;

out vec3 uvNormal;


void main(){


	vec4 position;
	mat4 modelViewMatrix = ViewProjectionMatrix *  ModelMatrix;

	if(Deffered)
	{
		position =  vec4(vertexPosition_modelspace,1);
	}
	else
	{
		position = modelViewMatrix * vec4(vertexPosition_modelspace,1);
	}

	

	
	
	uvNormal = vertexPosition_modelspace;

	gl_Position = position;
}

