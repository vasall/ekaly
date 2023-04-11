#version 460

layout(location=0) in vec3 vtxPos;
layout(location=1) in vec2 vtxTex;

layout(location=0) out vec2 uv;

void main()
{
	gl_Position = vec4(vtxPos.x, vtxPos.y, vtxPos.z, 1.0); 

	uv = vtxTex;
}
