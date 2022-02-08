#include "Texture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Texture::Initialized = false;
vector<Texture> Texture::AllTextures = vector<Texture>();


Texture::Texture( string Name, uint WrapStyle, uint FilterStyleMin, uint FilterStyleMag, vector<float> BorderColor ) :
	Name( Name )
{
	stbi_set_flip_vertically_on_load( true );

	if ( WrapStyle != 0 )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapStyle );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapStyle );
		if ( WrapStyle == GL_CLAMP_TO_BORDER )
		{
			Assert( BorderColor.size() == 4 );
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, BorderColor.data() );
		}
	}

	if ( FilterStyleMin != 0 )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterStyleMin );
	if ( FilterStyleMag != 0 )
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterStyleMag );

	int width, height, nrChannels;
	string FilePath = Path + "textures\\" + Name;
	unsigned char *data = stbi_load( FilePath.c_str(), &width, &height, &nrChannels, 0 );

	_ASSERTE( data );
	if ( stbi_failure_reason() )
		std::cout << stbi_failure_reason() << std::endl;

	ID = 0;
	GLuint Format = nrChannels == 3 ? GL_RGB : GL_RGBA;
	glGenTextures( 1, &ID );
	glBindTexture( GL_TEXTURE_2D, ID );
	glTexImage2D( GL_TEXTURE_2D, 0, Format, width, height, 0, Format, GL_UNSIGNED_BYTE, data );
	glGenerateMipmap( GL_TEXTURE_2D );
	stbi_image_free( data );

}
Texture::Texture() :
	ID( -1 ), Name( "" )
{}

const bool Texture::operator==( const Texture &Other )
{
	return ID == Other.ID && Name == Other.Name;
}
const bool Texture::operator!=( const Texture &Other )
{
	return ID != Other.ID || Name != Other.Name;
}

void Texture::Setup()
{
	for ( const auto &entry : std::filesystem::directory_iterator( Path + "textures" ) )
	{
		std::string fn = entry.path().string();
		AllTextures.push_back( Texture( entry.path().filename().string() ) );
	}
}
Texture Texture::Find( string Name )
{
	if ( !Initialized )
	{
		Initialized = true;
		Texture::Setup();
	}

	for ( int i = AllTextures.size(); --i >= 0; )
	{
		if ( AllTextures[ i ].Name == Name )
			return AllTextures[ i ];
	}
	Assert( Name == "" );
	return Texture();
}
void Texture::Terminate()
{
	for ( int i = AllTextures.size(); --i >= 0; )
	{
		glDeleteTextures( 1, &AllTextures[ i ].ID );
	}
}