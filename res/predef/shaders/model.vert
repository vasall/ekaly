#version 420

layout(location=0) in vec3 v_pos;
layout(location=1) in vec2 v_uv;
layout(location=2) in vec3 v_nrm;

layout(binding=0) uniform UBO {
	mat4 u_pos;
	mat4 u_rot;
	mat4 u_view;
	mat4 u_proj;
};

layout(location=0)out vec2 uv;
layout(location=1)out vec3 nrm;

void main()
{
	vec4 rotnrm;

	gl_Position = u_proj * u_view * u_pos * u_rot * vec4(v_pos, 1.0); 

	uv = v_uv;
	
	rotnrm = u_rot * vec4(v_nrm, 1.0);
	nrm = rotnrm.xyz;
}
