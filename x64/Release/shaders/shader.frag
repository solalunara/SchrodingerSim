#version 330 core

out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D tex;

void main()
{
	vec4 TexColor = texture( tex, TexCoord );
	if ( TexColor.a < .1 )
		discard;
	FragColor = TexColor;
}