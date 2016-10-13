#version 150

// Vi sitter på an jäääävligt gammal version av OpenGL på våra NUCs...
// osäker om vi får skriva location = 0 i den verionsen.

uniform mat4 projection;
uniform mat4 view;

layout ( location = 0 ) in vec3 vPosition;

out vec4 position;

void main()
{
	position = vec4(vPosition, 1.0);
	gl_Position = projection * view * position;
}