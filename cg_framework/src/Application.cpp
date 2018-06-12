// Computer Graphics and Media Informatics
// University of Konstanz
// 2015

#include "Application.h"
#include "Global.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "TgaLoader.h"

/*
-----------------------------------------------------------------------------
							Application.cpp
-----------------------------------------------------------------------------

This class is responsible for the rendering. 
A short description of the methods:

render()		-	Render loop method that is called for each frame.
init()			-	Is called once when the program is started.
reloadShader()	-	Is called when 'F5' is pressed to reload the shaders.
onKey()			-	Handles keyboard inputs.


Note: The GL_ASSERT() function is simply used to catch possible errors
when calling OpenGL functions - this is for debug purposes and you do
not need to write it around every OpenGL call you make!

-----------------------------------------------------------------------------
*/

//Contain the values for the hsv colors
float m_hue;
float m_saturation;
float m_brightness;
float winkel = 0;
bool changeOthografic = true;

/**
	Constructor and destructor.
*/
Application::Application(int width, int height)	:
	m_width(width),
	m_height(height)
{

}

Application::~Application()
{

}


/**
	This method is called once. Use it to initialize shaders, vertex buffer objects, and elements that do not change during the render loop.
*/
void Application::init()
{
	// The shader constructor takes as argument the files in which the Vertex and the Fragment shader are defined.
	m_shader = new Shader("../Shader/Default33.vert.glsl", "../Shader/Default33.frag.glsl");
	// --> If this is not working for you, use the line below.
	//m_shader = new Shader("../Shader/Default31.vert.glsl", "../Shader/Default31.frag.glsl");

	// A vertex buffer object (which stores geometry) is loaded from an *.obj file.
	m_vbo = ObjLoader::createVertexBufferObject("../Data/simple_cube.obj");
	//m_vbo = ObjLoader::createVertexBufferObject("../Data/torus.obj");
	//m_vbo = ObjLoader::createVertexBufferObject("../Data/box.obj");

	//m_hue = 120;
	//m_saturation = 0.8;
	//m_brightness = 0.5;
}



/**
	This method is called when the key 'F5' is pressed. It can be used to reload the shader if changes have been made, without having to restart the whole program.
*/
void Application::reloadShader()
{
	delete m_shader;
	m_shader = new Shader("../Shader/Default33.vert.glsl", "../Shader/Default33.frag.glsl");
	// --> If this is not working for you, use the line below.
	//m_shader = new Shader("../Shader/Default31.vert.glsl", "../Shader/Default31.frag.glsl");
}



/**
	This method is called when a key is pressed / released. Use it to bind actions to keys.
*/
void Application::onKey(GLint key, GLint scancode, GLint action, GLint mods) {

	if (action == GLFW_PRESS)
	{
		// key 'M' toggles between rendering the mesh as a wire and rendering as solid
		if (key == 'M' && action == GLFW_PRESS)
		{
			m_showMesh = !m_showMesh;
		}

		if (key == 'O' && action == GLFW_PRESS)
		{
			changeOthografic = !changeOthografic;
		}

		// key '1' decreases the hue value by x
		if (key == '1' && action == GLFW_PRESS)
		{
			m_hue -= 10;

			if (m_hue < 0)
			{
				m_hue += 360;
			}
		}

		// key '2' increases the hue value by x
		if (key == '2' && action == GLFW_PRESS)
		{
			m_hue += 10;

			if (m_hue > 360)
			{
				m_hue -= 360;
			}
		}

		// key '3' decreases the saturation value by x
		if (key == '3' && action == GLFW_PRESS && m_saturation >= 0.05)
		{
			m_saturation -= 0.05; 
		}

		// key '4' increases the saturation value by x
		if (key == '4' && action == GLFW_PRESS && m_saturation <= 0.95)
		{
			m_saturation += 0.05;
		}

		// key '5' decreases the brightness value by x
		if (key == '5' && action == GLFW_PRESS && m_brightness >= 0.05)
		{
			m_brightness -= 0.05;
		}

		// key '6' increases the brightness value by x
		if (key == '6' && action == GLFW_PRESS && m_brightness <= 0.95)
		{
			m_brightness += 0.05;
		}

		// key 'F5' reloads shaders
		if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
		{
			reloadShader();
		}
	}
}



/**
	The render loop. This is called for each frame.
*/
void Application::render()
{
    
	// Setting up the viewport of the window.
	glViewport(0, 0, m_framebufferWidth, m_framebufferHeight);

	// The rendering canvas is cleared with the clear color. Also the depth buffer is cleared.
	glClearColor(1.0f, 1.0f, 1.0f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// When pressing 'M', it is possible to toggle between a wireframe rendering mode and a solid rendering.
	if (!m_showMesh)
	{
		// Wireframe line rendering.
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}
	else
	{
		// 'Normal' solid rendering.
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}


	// The transformation matrices are set up which define the projection, the view frustum, and the transformation/scaling/rotation of the model.
	mat4 projection = mat4::perspective(45.0f, (GLfloat)m_width / (GLfloat)m_height, 0.1f, 1000.0f);
	mat4 view = mat4::translate(vec3(0, 0, m_zoom)) * mat4::rotate(m_rotate.x, 1, 0, 0) * mat4::rotate(m_rotate.y, 0, 1, 0);
	mat4 model = mat4::identitiy();
	mat4 inf = view.inverse();
	float f = 1000.0f;
	float n = 0.1f;
	float fov = 45.0f;
	float zoom = tanf(fov * math_radians / 2.0f);
	mat4 orthographic =    mat4((float)m_width / (float) m_height, 0.0f, 0.0f, 0.0f,
								0.0f, (float)m_width/(float)m_height, 0.0f, 0.0f,
								0.0f, 0.0f, (-2)/(f-n), -(n + f) / (f - n),
							    0.0f, 0.0f, 0.0f, 1.0f)	;
	//mat4 orthographic = projection * mat4(1/n,0,0,0, 0,1/n,0,0, 0,0,0,-((n-f)/f*(f-n)*n), 0,0,-1,(((-f-n)*(n-f))/(2*f*(f-n)*n)) + ((f+n)/(2*f*n)));
	//mat4 orthographic = projection * model;
	
	winkel += 0.001f;
	//std::cout << winkel;
	if (winkel > 360)
	{
		winkel = 0;
	}


	// The shader is activated. The upcoming geometry will be passed through the Vertex and Fragment shaders.
	m_shader->bind();

		// Parameters can be transferred to the GPU as "uniform" variables (visible in all shaders).
		// Other types (similar for int):
		//	setf	->	1D float
		//	set2f	->	2D float
		//	set3f	->	3D float
		if (changeOthografic)
		{
			m_shader->setMatrix("matProjection", projection, GL_TRUE);
			m_shader->setMatrix("matOrtographic", mat4::identitiy(), GL_TRUE);
			m_shader->seti("flag", 0);
		}
		else
		{
			m_shader->setMatrix("matOrtographic", orthographic, GL_TRUE);
			m_shader->setMatrix("matProjection", mat4::identitiy(), GL_TRUE);
			m_shader->seti("flag", 1);
		}
	
		m_shader->setf("winkel",winkel);
		m_shader->setMatrix("matView", view, GL_TRUE);
		m_shader->setMatrix("matModel", model, GL_TRUE);
		m_shader->set4f("camPos", inf.a14, inf.a24, inf.a34, inf.a44);

		vec3 hsv_offsets;
		hsv_offsets.x = m_hue;
		hsv_offsets.y = m_saturation;
		hsv_offsets.z = m_brightness;

		//std::cout << "* Debug " << m_hue << std::endl;

		m_shader->set3f("hsv_user_input", hsv_offsets);

		//m_hue = 0;
		//m_saturation = 0;
		//m_brightness = 0;

		// The geometry is pushed through the rendering pipeline. Vertex shader is called for each vertex; fragment shader for each fragment (potential pixel).
		m_vbo->render();

	// The shader is deactivated.
	m_shader->release();
}
