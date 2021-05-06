#version 430 core

layout(location = 0) out vec4 vFragColor;

uniform sampler2D screenSampler;
uniform vec3 WindowDimension;

in vec3 uvNormal;

void main()
{  
  vec2 pixelFrac = vec2(1.0f/WindowDimension.x, 1.0f/WindowDimension.y );
  // vec2 uvPos = vec2(gl_FragCoord.xy * pixelFrac);

  vec2 uv  = vec2((uvNormal.x + 1)/2,(uvNormal.y + 1)/2);
 

  if(uv[0] > .01f && uv[0] < .99f && uv[1] > .01f && uv[1] < .99f)  
  	vFragColor = texture(screenSampler, uv);
  else
  	vFragColor = vec4(1,0,0,1);
}
