#version 330 core

in vec3 position;
in vec2 texcoord;

uniform vec3 iota;

out vec2 Texcoord;

const int w = 0, h = 1;
const int x = 0, y = 1, z = 2;

void main()
{
	vec3 ndm = vec3(
		-1.0 + (position[x] * iota[x]),
		1.0 - (position[y] * iota[y]),
		-1.0 + (position[z] * iota[z])
	);

	Texcoord = texcoord;
	gl_Position = vec4(position, 1.0);
}
