layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vTangent;   // potentially unused
layout(location = 4) in vec3 vBitangent;

uniform mat4 ModelMatrix;      // local->world matrix
uniform mat4 ViewMatrix;       // world->eye matrix
uniform mat4 ProjectionMatrix; // eye->NDC matrix
