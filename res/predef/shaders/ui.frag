#version 460

layout(location=0) in vec2 uv;

layout(binding=1) uniform sampler2D tex;

layout(location=0) out vec4 FragColor;

void main()
{
	FragColor = texture(tex, uv);
}
