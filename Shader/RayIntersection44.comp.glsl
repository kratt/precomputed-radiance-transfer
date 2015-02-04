//Author: Julian Kratt
//Date: 03.02.2015

#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable


struct MeshFace
{
	vec4 va;
	vec4 vb;
	vec4 vc;
};


layout ( std140, binding = 0 ) buffer Data 
{
	MeshFace faces[];
};

layout ( std140, binding = 1 ) buffer Res 
{
	vec4 result[];
};

layout (binding = 2, offset = 0) uniform atomic_uint hitCounter;

layout (local_size_x = 1) in;


uniform vec3 p;
uniform vec3 dir;

bool rayIntersectsTriangle(vec3 p, vec3 dir, MeshFace face, out float dist)
{
	vec3 va = face.va.xyz;
	vec3 vb = face.vb.xyz;
	vec3 vc = face.vc.xyz;
	
	vec3 e1 = vb - va;
	vec3 e2 = vc - va;

	vec3 h = cross(dir, e2);

	float a = dot(e1, h);
	if (a > -0.00001f && a < 0.00001f)
		return false;


	float f = 1.0f / a;

	vec3 s = p - va;
	float u = f * dot(s, h);

	if (u < 0.0f || u > 1.0f)
		return false;

	vec3 q = cross(s, e1);
	float v = f * dot(dir, q);

	if (v < 0.0f || u + v > 1.0f)
		return false;
					
	float t = dot(e2, q)*f;
	if (t < 0.0f)
		return false;

	dist = t;
	return true;
}

void main()
{	   
	uint idx = gl_GlobalInvocationID.x;
	
	MeshFace curFace = faces[idx];
	
	float dist = 0.0f;
	bool hit = rayIntersectsTriangle(p, dir, curFace, dist);
	
	if(hit)
		atomicCounterIncrement(hitCounter);
	
	// if(hit && idx == 0)
	// {
		// result[1-idx].x = 1.0f;
		// result[1-idx].y = dist;
	// }
	// else if(idx== 0)
	// {
		// result[1-idx].x = 0.0f;
		// result[1-idx].y = 0.0f;
	// }
	
	// if(hit && idx == 1)
	// {
	
		// result[1-idx].x = 1.0f;
		// result[1-idx].y = dist;
	// }
	// else if(idx == 1)
	// {
		// result[1-idx].x = 0.0f;
		// result[1-idx].y = 0.0f;
	// }
	
	// result[0].z = idx;
	

	//memoryBarrierShared();
}






















