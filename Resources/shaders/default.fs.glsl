#version 460 core

in vec2 tex_coord;

out vec4 frag_color;

void main()
{
	frag_color = vec4(tex_coord, 1.0, 1.0);
}