#version 330 core

in vec3 position;
in vec2 texcoord;

uniform vec3 stageDim;

out vec2 Texcoord;

void main()
{
	float x = 2.0 * (position[0] / stageDim[0]) - 1.0;
	float y = 1.0 - ((2.0f * position[1] / stageDim[1]));

	Texcoord = texcoord;
	gl_Position = vec4(x, y, 0.5, 1.0);
}
