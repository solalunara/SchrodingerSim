#ifndef TEXTURE_H
#define TEXTURE_H

#pragma once

#include "Definitions.h"

class CPPRENDER_API Texture
{
public:
	Texture( string Name, uint WrapStyle = 0, uint FilterStyleMin = 0, uint FilterStyleMag = 0, vector<float> BorderColor = vector<float>() );
	Texture();

	string Name;
	uint ID;

	static Texture Find( string path );
	static void Terminate();

	const bool operator ==( const Texture &Other );
	const bool operator !=( const Texture &Other );


private:
	static bool Initialized;
	static void Setup();
	static vector<Texture> AllTextures;
};

#endif