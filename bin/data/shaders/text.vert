#version 130

in vec2 vertex;
in vec2 uv;

uniform mat4 matrix;
//uniform mat4 view;
uniform mat4 projection;

out vec2 uvCoords;

void main()
{
    uvCoords = uv;
	//gl_Position = (projection*view*matrix)*vec4(vertex,0,1);
	gl_Position = (projection*matrix)*vec4(vertex,0,1);
}
