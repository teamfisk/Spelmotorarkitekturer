#version 440

uniform mat4 projection;
uniform mat4 view;
uniform mat4 world;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
layout (location = 3) in vec3 binormal;
layout (location = 4) in vec2 vUV;
layout (location = 5) in vec4 boneIndices;
layout (location = 6) in vec4 boneWeights;

out vec4 position;
out vec2 uv;

void main()
{
	position = vec4(vPosition, 1.0);
	gl_Position = projection * view * world * position;
	uv = vUV;
}