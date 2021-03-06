//Author: Julian Kratt
//Date: 03.02.2015

#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

uniform vec3 rayOrigin;
uniform int rayOriginID;

struct MeshFace
{
	vec4 va;
	vec4 vb;
	vec4 vc;
	vec4 ids;
};

layout ( std140, binding = 0 ) buffer FaceBuff 
{
	MeshFace faces[];
};

layout ( std140, binding = 1 ) buffer SamplerDirBuff 
{
	vec4 sampleDirs[];
};

layout ( binding = 2 ) buffer HitBuff 
{
	float hitBuffer[];
};

layout ( binding = 3 ) buffer HitCountBuff 
{
	float hitCountBuffer[];
};


layout (local_size_x = 39, local_size_y = 39, local_size_z = 1) in;


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
	MeshFace face = faces[gl_GlobalInvocationID.x];
		
	uint idxA = uint(face.va.w);
	uint idxB = uint(face.vb.w);
	uint idxC = uint(face.vc.w);
	uint faceIdx      = uint(face.ids.x);
	
	uint sampleDirIdx = uint(sampleDirs[gl_GlobalInvocationID.y].w);
	
	vec4 curSampleDir = sampleDirs[sampleDirIdx];
	vec3 va = face.va.xyz;
	vec3 vb = face.vb.xyz;
	vec3 vc = face.vc.xyz;
	
	float eps = 0.000001f;

	if(rayOriginID == idxA || rayOriginID == idxB || rayOriginID == idxC ||
	   length(rayOrigin-va) < eps || length(rayOrigin-vb) < eps || length(rayOrigin-vc) < eps)
		return;
	
	float dist = 0.0f;
	bool hit = rayIntersectsTriangle(rayOrigin, curSampleDir.xyz, face, dist);
	
	uint hitIdx = sampleDirIdx;
						
	if(hit)	
	{
		//hitCountBuffer[rayOriginID] += 1;
		hitBuffer[hitIdx] = dist;	
	}
}






















