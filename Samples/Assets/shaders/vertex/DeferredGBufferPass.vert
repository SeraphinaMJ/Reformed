#version 430 core

#include "material_dependencies.glsl"

out vec3 WorldNormal;
out gl_PerVertex { vec4 gl_Position; float gl_PointSize;};
    
void main()
{
    WorldNormal = transpose(inverse(mat3(ModelMatrix)))  * vNormal;
    
    const vec4 fragPosition = ModelMatrix * vec4(vPosition, 1);
    
    const vec4 clipPos = ProjectionMatrix * ViewMatrix * fragPosition;
    
    gl_Position = clipPos;
}
