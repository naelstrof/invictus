#version 130

in vec2 vertex;
in vec2 uv;

out vec2 uvCoords;

void main()
{
    uvCoords = uv;
	gl_Position = vec4(vertex,0,1);
}
