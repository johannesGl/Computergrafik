#version 330 core

// Input variables from the previous shader stage.
in vec4 VertPosition;
in vec4 VertNormal;
in vec4 VertColor;
in vec4 VertTexture;
in vec3 VertEyeVec;
in vec3 VertLightVec;

// Uniform variable: passed to the shader from CPU side.
uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float shininess;
uniform vec3 LightColor;

// Output variable which determines the final color of the fragment.
out vec4 FragColor;

void main()
{
   //vec3 Diffuse = LightColor * dot(normalize(VertLightVec),normalize(VertNormal.xyz));
   //vec3 Specular = LightColor * pow(dot(normalize(reflect(VertLightVec,VertNormal.xyz)),normalize(VertEyeVec)),shininess);
   
   //vec4 color = VertColor;
  
   
   vec4 color = vec4(Ambient 
   + LightColor * (Diffuse * (dot(normalize(VertLightVec),normalize(VertNormal.xyz)))) 
   + Specular * pow((dot(normalize(VertLightVec + VertEyeVec),normalize(VertNormal.xyz))),shininess) , 1.0f);
   //vec4 color =  vec4((Ambient + Diffuse + Specular),1.0f);
   FragColor = vec4(color);	
}


