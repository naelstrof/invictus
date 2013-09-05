#version 120

uniform sampler2D texture;

varying vec2 uvCoord;

void main()
{
    gl_FragColor = texture2D( texture, uvCoord );
}
