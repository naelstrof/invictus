#version 130

in vec2 vertex;
in vec2 uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 world;

out vec2 uvCoords;

void main()
{
    uvCoords = uv;
	gl_Position = (world*view*model)*vec4(vertex,0,1);
}
