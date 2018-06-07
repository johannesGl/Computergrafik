#version 330 core

// Defines the order of the data in the vertex buffer object
#define VERT_POSITION	0
#define VERT_NORMAL     1
#define VERT_COLOR		2
#define VERT_TEXTURE    3

// Uniform variables: passed to the shader from CPU side.
uniform mat4x4 matModel;
uniform mat4x4 matView;	
uniform mat4x4 matProjection;
uniform mat4x4 matOrtographic;
uniform float winkel;
uniform int flag;


// Maps the data locations to variables.
layout(location = VERT_POSITION) in vec4 Position;
layout(location = VERT_NORMAL)   in vec4 Normal;
layout(location = VERT_COLOR)    in vec4 Color;
layout(location = VERT_TEXTURE)  in vec4 Texture;

// Output variables which are written to the next shader.
out vec4 VertPosition;
out vec4 VertNormal;
out vec4 VertColor;
out vec4 VertTexture;


void main()
{	   
    VertPosition = Position; 
    VertNormal   = Normal;
	VertColor    = Color;	
	VertTexture  = Texture;
	
	mat3x3 rotation = mat3x3(cos(winkel),0,-sin(winkel), 0,1,0, sin(winkel),0,cos(winkel));
	vec3 pos = rotation * VertPosition.xyz;
	if(flag == 0)
	{
		vec3 pos = rotation * VertPosition.xyz;
	}
	else
	{
		vec3 pos = VertPosition.xyz * rotation;
	}
	
	
	// OpenGL Macro that sets the position of the emitted geometry.
    gl_Position = (matProjection * matOrtographic) * matView * matModel * vec4(pos, 1);
}
