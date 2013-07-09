#version 120

uniform sampler2D texture;
//uniform vec4 Color;

varying vec2 uvCoords;

void main()
{
	//gl_FragColor = Color*texture2D(Texture, UVCoords);
    vec4 t = texture2D(texture,uvCoords);
    vec4 NewColor = vec4(0,0,0,t.r);
	gl_FragColor = NewColor;
}
