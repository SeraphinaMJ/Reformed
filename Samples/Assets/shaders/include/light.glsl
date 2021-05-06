// in order to include this header, you must define a constant LIGHT_COUNT
// which is the number of lights you would like to process at a time

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light
{
  bool isActive;
  bool ifDecay;
  vec4 position;
  vec4 direction; 
  vec4 ambient;  // ambient light cast onto objects
  vec4 diffuse;  // diffuse light cast onto objects
  float phi;     // innerAngle
  float theta;   // outerAngle
  float falloff; // for spotlight

  vec4 specular;
  int type;
  float intensity;
  vec3 disAtten;
};

