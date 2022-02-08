#ifndef DEF_H
#define DEF_H
#pragma once

/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>
*/

#include <crtdbg.h>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::vector;
using std::string;
using namespace glm;

#ifdef CPPRENDER_EXPORTS
#define CPPRENDER_API __declspec( dllexport )
#else
#define CPPRENDER_API __declspec( dllimport )
#endif

#define Assert( a ) _ASSERTE( a )
typedef unsigned int uint;





typedef vec3 Point3;
typedef vec2 Point2;

#ifdef _DEBUG
const string Path = std::filesystem::current_path().parent_path().string() + "\\x64\\Debug\\";
#else
const string Path = std::filesystem::current_path().string() + "\\";
#endif



#endif