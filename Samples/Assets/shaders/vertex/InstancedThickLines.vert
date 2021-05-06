#version 430 core

layout(location = 0) in vec3  vPosition;
// instanced attributes
layout(location = 1) in vec3  vNormal;
layout(location = 2) in vec3  vBegin;
layout(location = 3) in vec3  vEnd;
layout(location = 4) in vec3  vColor;
layout(location = 5) in float vWeight;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix; // local->NDC matrix

out gl_PerVertex { vec4 gl_Position; float gl_PointSize;};

out vec4 fragPosition;
out vec4 WorldNormal;

out flat vec4 color;

mat4 modelTransform()
{
    vec3 vec = vEnd - vBegin;
    vec3 forward = normalize(vec);
    vec3 right;
    vec3 up;
    
    vec3 rel_up = vec3(0,1,0);
    if(abs(abs(dot(forward, rel_up)) -  1) <= 0.0001)
    {
        right   = vec3(1, 0, 0);
        up      = vec3(0, 0, 1);
        forward = vec3(0, -1, 0);
    }
    else
    {
        right = cross(rel_up, forward);
        up    = cross(forward, right);
    }
    
    const vec3 offset = 0.5 * (vBegin + vEnd);
    
    const mat4 rot = mat4(
        vec4(right,   0),
        vec4(up,      0),
        vec4(forward, 0),
        vec4(offset,  1)
    );
    
    const mat4 scale = mat4(
        vec4(vWeight, 0, 0, 0),
        vec4(0, vWeight, 0, 0),
        vec4(0, 0, 0.5 * length(vec), 0),
        vec4(0, 0, 0, 1)
    );
    
    return rot * scale;
}

void main()
{
    const mat4 ModelMatrix = modelTransform();
    fragPosition = ModelMatrix * vec4(vPosition, 1);
    WorldNormal = vec4(transpose(inverse(mat3(ModelMatrix)))  * vNormal, 0);
    gl_Position = ProjectionMatrix * ViewMatrix * fragPosition;
    
    color = vec4(vColor,1);
}