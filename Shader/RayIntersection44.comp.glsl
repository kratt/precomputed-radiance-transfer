//Author: Julian Kratt
//Date: 03.02.2015

#version 430
#extension GL_ARB_compute_shader : enable
#extension GL_ARB_shader_storage_buffer_object : enable

uniform float numVertices;

struct MeshFace
{
	vec4 va;
	vec4 vb;
	vec4 vc;
};


layout ( std140, binding = 0 ) buffer VertBuff 
{
	vec4 vertices[];
};

layout ( std140, binding = 1 ) buffer FaceBuff 
{
	MeshFace faces[];
};

layout ( std140, binding = 2 ) buffer SamplerDirBuff 
{
	vec4 sampleDirs[];
};

layout ( binding = 3 ) buffer HitBuff 
{
	float hitBuffer[];
};


layout (local_size_x = 32, local_size_y = 32, local_size_z = 1) in;


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
	uint vertIdx      = uint(vertices[gl_GlobalInvocationID.x].w);
	uint faceIdx      = uint(faces[gl_GlobalInvocationID.y].va.w);
	uint sampleDirIdx = uint(sampleDirs[gl_GlobalInvocationID.z].w);
	
	MeshFace curFace  = faces[faceIdx];
	vec4 curVertex    = vertices[vertIdx];
	vec4 curSampleDir = sampleDirs[sampleDirIdx];
	
	
	float dist = 0.0f;
	bool hit = rayIntersectsTriangle(curVertex.xyz, curSampleDir.xyz, curFace, dist);
	
	uint realVertIdx = uint(curVertex.w);
	uint hitIdx = uint(sampleDirIdx * numVertices + vertIdx);
			
	if(hit)	
	{
		hitBuffer[hitIdx] = dist;	
	}
}






















