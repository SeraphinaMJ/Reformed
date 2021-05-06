#version 430 core

//// ScreenSpace Shader

layout(location = 0) out vec4 vFragColor;
	
in vec2 uv;

 uniform sampler2D ScreenSampler;

void main()
{  

	

  if(uv[0] > .01f && uv[0] < .99f && uv[1] > .01f && uv[1] < .99f){
	vFragColor = texture(ScreenSampler, uv);
  }
  else{
	vFragColor = vec4(1,0,0,1);
  }
  	
}
