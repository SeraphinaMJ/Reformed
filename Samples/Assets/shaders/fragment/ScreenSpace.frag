#version 430 core

//// ScreenSpace Shader

layout(location = 0) out vec4 vFragColor;
	
in vec2 uv;

 uniform sampler2D ScreenSampler;

void main()
{  

	
  	vFragColor = texture(ScreenSampler, uv);

  	//vFragColor = vec4(0.2667, 0.2667, 0.0392, 1.0);
}
