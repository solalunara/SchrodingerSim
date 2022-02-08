#include "Entity.h"
#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Face::~Face()
{
	glDeleteVertexArrays( 1, &VAO );
	glDeleteBuffers( 1, &VBO );
	glDeleteBuffers( 1, &EBO );
}

Entity::Entity( vec2 Mins, vec2 Maxs, Texture FrontTexture, Texture BackTexture, Transform transform ) :
	FrontTexture( FrontTexture ), BackTexture( BackTexture ), transform( transform ), Mins( Mins ), Maxs( Maxs ), 
	FrontFace( 0, 0, 0 ), BackFace( 0, 0, 0 )
{
	vec2 Size( Maxs - Mins );
	float Verts[ 20 ] = 
	{ 
		Mins.x, Mins.y, 0,		0.0f, 0.0f,
		Maxs.x, Mins.y, 0,		1.0f, 0.0f,
		Maxs.x, Maxs.y, 0,		1.0f, 1.0f,
		Mins.x, Maxs.y, 0,		0.0f, 1.0f
	};

	int CCWInds[ 6 ] = { 0, 1, 3, 1, 2, 3 };
	int  CWInds[ 6 ] = { 0, 3, 1, 1, 3, 2 };

	glGenVertexArrays( 1, &FrontFace.VAO );
	glBindVertexArray( FrontFace.VAO );
	glGenBuffers( 1, &FrontFace.VBO );
	glBindBuffer( GL_ARRAY_BUFFER, FrontFace.VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Verts ), Verts, GL_STATIC_DRAW );
	glGenBuffers( 1, &FrontFace.EBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, FrontFace.EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( CCWInds ), CCWInds, GL_STATIC_DRAW );

	// vertex positions
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
	glEnableVertexAttribArray( 0 );
	// texture coords
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)( 3 * sizeof( float ) ) );
	glEnableVertexAttribArray( 1 );

	glGenVertexArrays( 1, &BackFace.VAO );
	glBindVertexArray( BackFace.VAO );
	glGenBuffers( 1, &BackFace.VBO );
	glBindBuffer( GL_ARRAY_BUFFER, BackFace.VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Verts ), Verts, GL_STATIC_DRAW );
	glGenBuffers( 1, &BackFace.EBO );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, BackFace.EBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( CWInds ), CWInds, GL_STATIC_DRAW );

	// vertex positions
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)0 );
	glEnableVertexAttribArray( 0 );
	// texture coords
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( float ), (void *)( 3 * sizeof( float ) ) );
	glEnableVertexAttribArray( 1 );
}

void Entity::Draw( Shader *shader )
{
	shader->Set( "DirectControlMode", 0 );
	glActiveTexture( GL_TEXTURE0 );

	glBindTexture( GL_TEXTURE_2D, FrontTexture.ID );
	glBindVertexArray( FrontFace.VAO );
	glDrawElements( GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0 );

	glBindTexture( GL_TEXTURE_2D, BackTexture.ID );
	glBindVertexArray( BackFace.VAO );
	glDrawElements( GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0 );

	glBindTexture( GL_TEXTURE_2D, 0 );
}
bool Entity::TestPointCollision( vec2 Pt )
{
	vec3 LocalPt = transform.WorldToLocalPoint( vec3( Pt, 0 ) );
	return LocalPt.x >= Mins.x && LocalPt.y >= Mins.y && LocalPt.x <= Maxs.x && LocalPt.y <= Maxs.y;
}

TwoPointLine::TwoPointLine( dvec3 b, dvec3 a ) :
	a( a ), b( b ), face( 0, 0, 0 )
{
	float Verts[ 6 ] =
	{
		a.x, a.y, a.z,
		b.x, b.y, b.z,
	};

	glGenVertexArrays( 1, &face.VAO );
	glBindVertexArray( face.VAO );
	glGenBuffers( 1, &face.VBO );
	glBindBuffer( GL_ARRAY_BUFFER, face.VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Verts ), Verts, GL_DYNAMIC_DRAW );

	// vertex positions
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float ), (void *)0 );
	glEnableVertexAttribArray( 0 );
}
void TwoPointLine::Update( dvec3 b, dvec3 a )
{
	float Verts[ 6 ] =
	{
		(float)a.x, (float)a.y, (float)a.z,
		(float)b.x, (float)b.y, (float)b.z,
	};

	this->a = a;
	this->b = b;

	_ASSERTE( face.VAO );
	glBindVertexArray( face.VAO );
	_ASSERTE( face.VBO );
	glBindBuffer( GL_ARRAY_BUFFER, face.VBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( Verts ), Verts, GL_DYNAMIC_DRAW );
}
void TwoPointLine::Draw( Shader *shader, float Width )
{
	glUseProgram( shader->ID );
	glLineWidth( Width );
	glBindVertexArray( face.VAO );
	glDrawArrays( GL_LINES, 0, 2 );
}