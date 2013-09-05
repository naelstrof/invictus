#version 120

attribute vec2 vertex;
attribute vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 world;

varying vec2 uvCoord;

void main()
{
    uvCoord = uv;
	gl_Position = (world*view*model)*vec4(vertex,0,1);
}
