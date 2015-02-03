//Author: Julian Kratt
//Date: 03.02.2015

#version 440
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable


struct VertData
{
	vec4 VertPosition;
	vec4 VertNormal;
	vec4 VerColor;
	vec4 VertTexture;
};

layout ( std140 , binding=0 ) buffer Pos {
	VertData ParticleData[];
};

layout (local_size_x = 512) in;

void main()
{	   
	uint idx = gl_GlobalInvocationID.x;
	
	vec4 pos = ParticleData[idx].VertPosition;
	
	ParticleData[idx].VertPosition = pos + vec4(0.0, 0.001, 0.0, 0.0);
}
