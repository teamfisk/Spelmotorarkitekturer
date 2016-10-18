#version 150

uniform mat4 projection;
uniform mat4 view;

// Funkar inte i NUCens OpenGL ... vrf?
//layout ( location = 0 ) in vec3 vPosition;

in vec3 vPosition;

out vec4 position;

void main()
{
	position = vec4(vPosition, 1.0);
	gl_Position = projection * view * position;
}