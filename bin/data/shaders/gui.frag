#version 120

uniform sampler2D texture;
uniform vec4 color;

varying vec2 uvCoord;

void main()
{
    gl_FragColor = texture2D( texture, uvCoord ) * color;
}
