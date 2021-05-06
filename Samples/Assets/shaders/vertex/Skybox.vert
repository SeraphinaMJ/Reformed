#version 430 core

layout(location = 0) in vec3 vPosition;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix; // local->NDC matrix

out vec3 uvNormal;

void main()
{
    uvNormal = vPosition;
    
    const vec3 viewPos = mat3(ViewMatrix) * vPosition;
    gl_Position = (ProjectionMatrix * vec4(viewPos, 1)).xyww;
}
