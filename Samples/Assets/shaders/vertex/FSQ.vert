#version 430 core

layout(location = 0) in vec3 vPosition;

out vec2 UV;

out gl_PerVertex
{
    vec4 gl_Position;
    float gl_PointSize;
};

void main()
{
    // [-1,1] -> [0,2] -> [0,1]
    UV = 0.5 * ((vPosition.xy) + 1);
    gl_Position =  vec4(vPosition,1);
}
