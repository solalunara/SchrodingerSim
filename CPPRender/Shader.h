#ifndef SHADER_H
#define SHADER_H

#pragma once

#include "Definitions.h"
#include <iostream>
#include <fstream>
#include <sstream>


class CPPRENDER_API Shader
{
public:
	Shader( string VertPath, string FragPath );
	~Shader();

	uint ID;

	const void Set( const string &name, int value );
	const void Set( const string &name, float value );
	const void Set( const string &name, mat4 value );
	const void Set( const string &name, vector<vec4> values );
	const void Set( const string &name, vector<float> values );
};

#endif