#version 130

uniform sampler2D texture;
uniform vec4 color;

in vec2 uvCoords;
out vec4 finalColor;

void main()
{
    finalColor = texture2D( texture, uvCoords )*color;
}
