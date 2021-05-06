
uniform mat4 invView;
uniform vec2 depthrange;

vec3 calcPositionFromDepth(in mat4 invProj, in float depthZ, in vec2 UV)
{
    const float ndcZ = (2.0 * depthZ - depthrange.x - depthrange.y) /
    (depthrange.y - depthrange.x);
    const vec2 ndcXY = (UV * 2) - 1;
    
    const vec4 ndcPos = vec4(ndcXY, ndcZ, 1);
    const vec4 eyePos = invProj * ndcPos;
    const vec4 wPos = invView * (eyePos / eyePos.w);
    return wPos.xyz;
}
