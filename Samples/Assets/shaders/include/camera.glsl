uniform struct
{
    float fogAmmount;
    vec4 position;
    vec4 i_fog;
    float z_near;
    float z_far;
}camera;

vec3 camera_fog(in vec3 color, in vec4 fragPosition)
{
    const float fragDist = distance(camera.position, fragPosition);

    const float s = max((camera.z_far -      fragDist)  /
                        (camera.z_far - camera.z_near), 0);

    return mix(camera.i_fog.rgb, color, s);
}
