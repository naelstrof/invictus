#version 120

attribute vec2 vertex;
attribute vec2 uv;

uniform mat4 model;
uniform mat4 world;

varying vec2 uvCoord;

void main()
{
    uvCoord = uv;
	//gl_Position = (projection*view*matrix)*vec4(vertex,0,1);
	gl_Position = (world*model)*vec4(vertex,0,1);
}
