#include "Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

Shader::Shader( string VertPath, string FragPath )
{
	string VertCode;
	string FragCode;
	ifstream VShaderFile;
	ifstream FShaderFile;

	VShaderFile.exceptions( ifstream::failbit | ifstream::badbit );
	FShaderFile.exceptions( ifstream::failbit | ifstream::badbit );

	try
	{
		// open files
		VShaderFile.open( VertPath );
		FShaderFile.open( FragPath );
		stringstream VShaderStream, FShaderStream;
		// read file's buffer contents into streams
		VShaderStream << VShaderFile.rdbuf();
		FShaderStream << FShaderFile.rdbuf();
		// close file handlers
		VShaderFile.close();
		FShaderFile.close();
		// convert stream into string
		VertCode = VShaderStream.str();
		FragCode = FShaderStream.str();
	}
	catch ( const exception &e )
	{
		cout << e.what() << endl;
	}

	const char *VShaderCode = VertCode.c_str();
	const char *FShaderCode = FragCode.c_str();

	unsigned int Vertex, Fragment;
	int success;
	char infoLog[ 512 ];

	// vertex Shader
	Vertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( Vertex, 1, &VShaderCode, NULL );
	glCompileShader( Vertex );
	// print compile errors if any
	glGetShaderiv( Vertex, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( Vertex, 512, NULL, infoLog );
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};

	// similiar for Fragment Shader
	Fragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( Fragment, 1, &FShaderCode, NULL );
	glCompileShader( Fragment );
	// print compile errors if any
	glGetShaderiv( Fragment, GL_COMPILE_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( Fragment, 512, NULL, infoLog );
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};

	// shader Program
	this->ID = glCreateProgram();
	glAttachShader( ID, Vertex );
	glAttachShader( ID, Fragment );
	glLinkProgram( ID );
	// print linking errors if any
	glGetProgramiv( ID, GL_LINK_STATUS, &success );
	if ( !success )
	{
		glGetProgramInfoLog( ID, 512, NULL, infoLog );
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader( Vertex );
	glDeleteShader( Fragment );
}
Shader::~Shader()
{
	glDeleteProgram( ID );
}

const void Shader::Set( const string &name, int value ) { glUniform1i( glGetUniformLocation( ID, name.c_str() ), (int)value ); }
const void Shader::Set( const string &name, float value ) { glUniform1f( glGetUniformLocation( ID, name.c_str() ), value ); }
const void Shader::Set( const string &name, mat4 value ) { glUniformMatrix4fv( glGetUniformLocation( ID, name.c_str() ), 1, GL_FALSE, value_ptr( value ) ); }
const void Shader::Set( const string &name, vector<vec4> values )
{
	vector<float> floatvalues;
	for ( uint i = 0; i < values.size(); ++i )
		for ( uint j = 0; j < 4; ++j )
			floatvalues.push_back( values[ i ][ j ] );

	glUniform4fv( glGetUniformLocation( ID, name.c_str() ), (int)values.size(), floatvalues.data() );
}
const void Shader::Set( const string &name, vector<float> values ) { glUniform1fv( glGetUniformLocation( ID, name.c_str() ), 100, values.data() ); }