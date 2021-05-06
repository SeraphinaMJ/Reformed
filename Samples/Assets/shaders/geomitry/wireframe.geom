#version 430 core
#extension GL_EXT_gpu_shader4 : enable
#extension GL_EXT_geometry_shader4 : enable

layout (points) in;
noperspective varying vec3 dist;

void main()
{
    // taken from 'Single-Pass Wireframe Rendering'
    const vec2 p0 = gl_PositionIn[0].xy / gl_PositionIn[0].w;
    const vec2 p1 = gl_PositionIn[1].xy / gl_PositionIn[1].w;
    const vec2 p2 = gl_PositionIn[2].xy / gl_PositionIn[2].w;

    const vec2 v0 = p2 - p1;
    const vec2 v1 = p2 - p0;
    const vec2 v2 = p1 - p0;
    const float area = abs(v1.x * v2.y - v1.y * v2.x);

    dist = vec3(area / length(v0), 0 , 0 );
    
    EmitVertex();
    
    dist = vec3(0 , area / length(v1), 0 );
    
    EmitVertex();
    
    dist = vec3(0 , 0 , area / length(v2));
    
    EmitVertex();
    EndPrimitive();
}

