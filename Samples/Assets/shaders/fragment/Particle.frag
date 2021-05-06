#version 430 core

layout(location = 0) out vec4 vFragColor;

in vec4 color;
//in vec2 TexCoord;

uniform float glow;

uniform sampler2D ourTexture;

void main()
{  
 
  vFragColor = glow + color;
  //vFragColor = texture(ourTexture, TexCoord) * color;
}
