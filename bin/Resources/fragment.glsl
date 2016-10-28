#version 440

in vec4 position;
in vec2 uv;

layout (binding = 0/*will use the texture bound to GL_TEXTURE0*/) uniform sampler2D tex;

out vec4 color;

void main()
{
	//color = vec4(1.0, 1.0, 0.0, 1.0);
	//color = texture(tex, uv); // Any difference?
	color = texture2D(tex, uv);
}