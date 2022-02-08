#include "Window.h"
#include "Shader.h"
#include "Transform.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

bool Window::GLFWInitialized = false;
vector<Window*> Window::AllWindows = vector<Window*>();

Window::Window( int Width, int Height, string Name ) :
	ID( NULL ), s( NULL )
{
	if ( !GLFWInitialized )
	{
		GLFWInitialized = true;
		glfwInit();
	}
	//tell opengl we're opening up a window
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

	//initialize the rendering window
	ID = glfwCreateWindow( Width, Height, Name.c_str(), NULL, NULL);
	if ( ID == NULL )
	{
		_ASSERTE( false );
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent( ID );
	if ( !gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		_ASSERTE( false );
		return;
	}
	//set the viewport to render to
	glViewport( 0, 0, Width, Height );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_FRAMEBUFFER_SRGB );
	glEnable( GL_CULL_FACE );
	glDepthFunc( GL_LESS );

	glfwSetFramebufferSizeCallback( ID, WindowResized );

	s = new Shader( Path + "shaders/shader.vert", Path + "shaders/shader.frag" );

	AllWindows.push_back( this );
}
Window::~Window()
{
	delete s;
}

const mat4 Perspective = perspective( radians( 70.0f ), 1.0f, .00001f, 1000.0f );

void Window::StartFrame( Transform *Camera )
{
	glUseProgram( s->ID );
	glClearColor( .1f, .2f, .7f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	s->Set( "Perspective", Perspective );
	s->Set( "CameraTransform", inverse( Camera->GetMatrix() ) );
}
void Window::RenderObject( Entity *ent )
{
	s->Set( "transform", ent->transform.GetMatrix() );
	ent->Draw( s );
}
void Window::DrawLine( TwoPointLine *line, float Width )
{
	s->Set( "transform", identity<mat4>() );
	line->Draw( s, Width );
}
void Window::EndFrame()
{
	glfwSwapBuffers( ID );
	glfwPollEvents();
}

void Window::SetKeyboardCallback( void *f )
{
	glfwSetKeyCallback( ID, (GLFWkeyfun)f );
}
void Window::SetMouseCallback( void *f )
{
	glfwSetMouseButtonCallback( ID, (GLFWmousebuttonfun)f );
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose( ID );
}

void WindowResized( GLFWwindow *window, int width, int height )
{
	glViewport( 0, 0, width, height );
}
vec2 GetCursorPosition( GLFWwindow *window )
{
	double X, Y;
	glfwGetCursorPos( window, &X, &Y );
	return vec2( X, Y );
}
vec2 GetWindowSize( GLFWwindow *window )
{
	int X, Y;
	glfwGetWindowSize( window, &X, &Y );
	return vec2( (float)X, (float)Y );
}
float GetTime()
{
	return glfwGetTime();
}

Window *Window::FindWindowByID( GLFWwindow *ID )
{
	for ( int Index = 0; Index < AllWindows.size(); ++Index )
	{
		try
		{
			if ( AllWindows[ Index ]->ID == ID )
				return AllWindows[ Index ];
		}
		catch ( const char *Message ) //Access Violation : This window no longer exists, remove it from the vector
		{
			AllWindows.erase( AllWindows.begin() + Index );
			--Index;
		}
	}
	return NULL;
}