#version 330 core

// Input variables from the previous shader stage.
in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;

// Uniform variable: passed to the shader from CPU side.
uniform vec3 lightPos;
uniform vec4 CamPos;


// Output variable which determines the final color of the fragment.
out vec4 FragColor;

void main()
{
	float x = dot((CamPos-VertPosition),VertNormal);
   
   if(x<=0)
   {
	discard;
   }
   vec4 color = VertColor;
   FragColor = vec4(color);	
   
}