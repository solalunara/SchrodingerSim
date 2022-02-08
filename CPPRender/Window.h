#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include "Definitions.h"
#include "Entity.h"
#include <thread>

class Shader;
struct GLFWwindow;

class CPPRENDER_API Window
{
public:
	Window( int Width, int Height, string Name );
	~Window();

	void StartFrame( Transform *Camera );
	void RenderObject( Entity *ent );
	void DrawLine( TwoPointLine *line, float Width );
	void EndFrame();

	bool IsOpen();
	//GLFWkeyfun
	void SetKeyboardCallback( void *f );
	//GLFWmousebuttonfun
	void SetMouseCallback( void *f );

	static Window *FindWindowByID( GLFWwindow *ID );

private:
	GLFWwindow *ID;
	Shader *s;

	static bool GLFWInitialized;
	static vector<Window*> AllWindows;
};

void WindowResized( GLFWwindow *window, int width, int height );

CPPRENDER_API vec2 GetCursorPosition( GLFWwindow *window );
CPPRENDER_API vec2 GetWindowSize( GLFWwindow *window );
CPPRENDER_API float GetTime();

CPPRENDER_API extern const mat4 Perspective;

#endif