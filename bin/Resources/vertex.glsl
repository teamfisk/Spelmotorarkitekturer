#version 440

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

// Funkar inte i NUCens OpenGL ... vrf?
//layout ( location = 0 ) in vec3 vPosition;

in vec3 vPosition;
in vec2 vUV;

out vec4 position;
out vec2 uv;

void main()
{
	position = vec4(vPosition, 1.0);
	gl_Position = projection * view * world * position;
	uv = vUV;
}