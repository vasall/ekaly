#version 460

layout(location=0) in vec3 vtxPos;
layout(location=1) in vec2 vtxTex;

layout(binding=0) uniform UBO {
	mat4 view;
	mat4 proj;
};

layout(location=0) out vec2 uv;

void main()
{

	gl_Position = proj * view * vec4(vtxPos, 1.0); 

	uv = vtxTex;
}
