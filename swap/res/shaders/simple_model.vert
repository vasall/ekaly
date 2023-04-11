#version 420

layout(location=0) in vec3 vtxPos;
layout(location=1) in vec2 vtxTex;
layout(location=2) in vec3 vtxNrm;


layout(location=0) out vec2 uv;
layout(location=1) out vec3 nrm;

void main()
{
	vec4 rotnrm;

	gl_Position = vec4(vtxPos.x, vtxPos.y, vtxPos.z + 10, 1.0); 

	uv = vtxTex;
	
	rotnrm = vec4(vtxNrm, 1.0);
	nrm = rotnrm.xyz;
}
