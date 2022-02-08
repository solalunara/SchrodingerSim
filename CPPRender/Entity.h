#ifndef ENT_H
#define ENT_H

#pragma once

#include "Texture.h"
#include "Transform.h"

class Shader;

struct Face
{
	Face( uint VAO, uint VBO, uint EBO ) : VAO( VAO ), VBO( VBO ), EBO( EBO ) { }
	~Face();
	uint VAO;
	uint VBO;
	uint EBO;
};

class CPPRENDER_API Entity
{
public:
	Entity( vec2 Mins, vec2 Maxs, Texture FrontTexture, Texture BackTexture, Transform transform );

	Texture FrontTexture;
	Texture BackTexture;

	Transform transform;

	virtual void Draw( Shader *shader );

	bool TestPointCollision( vec2 Pt );

private:
	Face FrontFace;
	Face BackFace;

	vec2 Mins;
	vec2 Maxs;
};

class CPPRENDER_API TwoPointLine
{
public:
	TwoPointLine( dvec3 b, dvec3 a );

	void	Update( dvec3 b, dvec3 a );
	void	Draw( Shader *shader, float Width );

	dvec3	a;
	dvec3	b;

private:
	Face face;
};

#endif