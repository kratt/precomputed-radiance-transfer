//Author: Julian Kratt
//Date: 03.02.2015

#version 440
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
	vec4 result;
};

layout (local_size_x = 128) in;


uniform vec3 p;
uniform vec3 dir;

bool intersectTriangle(vec3 orig, vec3 dir, MeshFace face, out float dist)
{
    vec3 u, v, n; // triangle vectors
    vec3 w0, w;  // ray vectors
    float r, a, b; // params to calc ray-plane intersect

	vec3 va = face.va.xyz;
	vec3 vb = face.vb.xyz;
	vec3 vc = face.vc.xyz;
	
    // get triangle edge vectors and plane normal
    u = vb - va;
    v = vc - va;
    n = cross(u, v);

    w0 = orig - va;
    a = -dot(n, w0);
    b = dot(n, normalize(dir));
    if (abs(b) < 1e-5)
    {
        // ray is parallel to triangle plane, and thus can never intersect.
        return false;
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)
        return false; // ray goes away from triangle.

    vec3 I = orig + r * dir;
    float uu, uv, vv, wu, wv, D;
    uu = dot(u, u);
    uv = dot(u, v);
    vv = dot(v, v);
    w = I - va;
    wu = dot(w, u);
    wv = dot(w, v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)
        return false;
		
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return false;

    if (r < 1e-5) 
		return false;
		
	dist = r;
	return true;
}

void main()
{	   
	uint idx = gl_GlobalInvocationID.x;
	
	MeshFace curFace = faces[idx];
	
	float dist = 0.0f;
	bool hit = intersectTriangle(p, dir, curFace, dist);
	
	if(hit)
	{
		result.x = 1.0f;
		// vec4 prevRes = result;
		// float prevDist = result.x;
		
		// if(dist < preDist) // we have a new closer intersection
		// {
			
		// }
	}
}






















