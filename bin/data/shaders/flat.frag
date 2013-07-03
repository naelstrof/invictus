#version 120

uniform sampler2D Texture;
uniform vec4 Color;

varying vec2 UVCoords;

void main()
{
	gl_FragColor = Color*texture2D(Texture, UVCoords);
}
