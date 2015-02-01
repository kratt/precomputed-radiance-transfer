//Author: Sören Pirk
//Date: 22.01.2013

#include "Picking.h"
#include "Camera.h";
#include "Frustum.h"

Picking::Picking()
{
}

Picking::~Picking()
{
}

vec3 Picking::pickingRay(Camera* camera, int mouse_x, int mouse_y, float window_width, float window_height)
{
	double aspect = double(window_width)/double(window_height);
	Frustum* frustum = camera->frustum();
	
	float near_height = frustum->nh;

	int window_y = (window_height - mouse_y) - window_height/2; 
	double norm_y = double(window_y)/double(window_height/2); 
	int window_x = mouse_x - window_width/2; 
	double norm_x = double(window_x)/double(window_width/2);

	float y = near_height * norm_y; 
	float x = near_height * aspect * norm_x;

	float zNear = frustum->nearD;

	return vec3(x, y, -zNear);
}

//http://schabby.de/picking-opengl-ray-tracing/
void Picking::getPickingRay(const Transform &trans, const float fov, const float ncp, const float window_width, const float window_height, float mouse_x, float mouse_y, vec3 &rayPos, vec3 &rayDir)
{
	vec3 camPos  = getCamPosFromModelView(trans);
	vec3 viewDir = getViewDirFromModelView(trans);
	vec3 upDir   = getUpDirFromModelView(trans);

	double aspect = double(window_width)/double(window_height);

	vec3 view = normalize(viewDir);
    
    vec3 tmpa = cross(view, upDir);
	vec3 h = normalize(tmpa);
    
    vec3 tmpb = cross(h, view);
	vec3 v = normalize(tmpb);

	float rad = fov * math_radians;
	float vLength = tan( rad / 2.0 ) * ncp;
	float hLength = vLength * (window_width / window_height);

	v *= vLength;
	h *= hLength;

	// translate mouse coordinates so that the origin lies in the center of the view port
	mouse_y = window_height - mouse_y;

	mouse_x -= window_width / 2.0f;
	mouse_y -= window_height / 2.0f;

	// scale mouse coordinates so that half the view port width and heigh becomes 1
	mouse_y /= (window_height / 2.0f);
	mouse_x /= (window_width / 2.0f);

	// linear combination to compute intersection of picking ray with view port plane
	vec3 pos = camPos + view*ncp + h*mouse_x + v*mouse_y;
    vec3 tmp = pos - camPos;
	vec3 dir = normalize(tmp);

	rayPos = camPos;
	rayDir = dir;
}

vec3 Picking::intersectionPlane(vec3 v1, vec3 v2, vec3 v3, vec3 startPos, vec3 endPos)
{
	//vec3 n = (v3-v2).cross(v1-v2);
	//float b  =  v2.dot(n);
	//float length = n.length();


	//Vector3 s;
	//Vector3 dir = endPos - startPos;
	//float angle = n.angle(dir);

	//float dist_start = abs((n.dot(startPos) - b) / length);
	//float dist_end   = abs((n.dot(endPos) - b) / length);

	//if(angle != math_pi/2.0f)
	//{   // not parallel
	//		float b = v2.x*n.x + v2.y*n.y + v2.z*n.z;
	//		float tmp = n.dot(dir);

	//		if(tmp != 0.0f)
	//		{
	//			float t = (b - n.dot(startPos)) / tmp ;
	//			if(0.0f<= t && t <=1.0f)
	//			{

	//				if(dist_end < dist_start)
	//				{
	//					s = startPos + t*dir;	
	//				}
	//			}

	//		}
	//}

	//return s;

	vec3 n = (v3-v2).cross(v1-v2);
	float b  =  v2.dot(n);
	float length = n.length();

	Vector3 s;
	Vector3 dir = endPos - startPos;
	float angle = n.angle(dir);

	float dist_start = std::abs((n.dot(startPos) - b) / length);
	float dist_end   = std::abs((n.dot(endPos) - b) / length);

	if(angle != math_pi/2.0f)
	{   // not parallel
		float b = v2.x*n.x + v2.y*n.y + v2.z*n.z;
		float tmp = n.dot(dir);

		if(tmp != 0.0f)
		{
			float t = (b - n.dot(startPos)) / tmp ;
			//if(0.0f<= t && t <=1.0f)
			{

				//if(dist_end < dist_start)
				{
					s = startPos + t*dir;	

				}
			}
		}
	}

	return s;
}

bool Picking::intersectTriangle(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, float &t)
{
	float eps = 0.000000001f;

	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;

	vec3 p = cross(rayDir, e2);
	float a = dot(e1, p);

	if(a > -eps && a < eps)
	{
		return false;
	}

	float f = 1.0f / a;
	vec3 s = rayStart - v0;
	float u = f * dot(s,p);

	if(u < 0.0f || u > 1.0f)
		return false;

	vec3 q = cross(s, e1);
	float v = f * dot(rayDir, q);

	if(v < 0.0f || u+v > 1.0f)
		return false;

	t = f * dot(e2, q);

	return true;
}

bool Picking::intersectQuad(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, vec3 v3, float &t)
{
	float t1 = math_maxfloat;
	float t2 = math_maxfloat;

	bool b1 = intersectTriangle(rayStart, rayDir, v3, v1, v2, t1);
	bool b2 = intersectTriangle(rayStart, rayDir, v3, v0, v1, t2);

	t = g_min<float>(t1,t2);
	return b1 || b2;	
}

bool Picking::intersectCube(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, float &t)
{
	t = math_maxfloat;
	float s = math_maxfloat;
	bool b0, b1, b2, b3, b4, b5;

	// back
	if(b0 = intersectQuad(rayStart, rayDir, v0, v1, v2, v3, s)) 
		t =  g_min<float>(t, s);

	//front
	if(b1 = intersectQuad(rayStart, rayDir, v4, v5, v6, v7, s))
		t = g_min<float>(t, s);
	

	// left
	if(b2 = intersectQuad(rayStart, rayDir, v0, v4, v7, v3, s))
		t = g_min<float>(t, s);
	
	// right
	if(b3 = intersectQuad(rayStart, rayDir, v5, v1, v2, v6, s))
		t = g_min<float>(t, s);
	
	// top
	if(b4 = intersectQuad(rayStart, rayDir, v7, v6, v2, v3, s))
		t = g_min<float>(t, s);


	// bottom
	if(b5 = intersectQuad(rayStart, rayDir, v4, v5, v1, v0, s))
		t = g_min<float>(t, s);

	return b0 || b1 || b2 || b3 || b4 || b5;
}


float Picking::select(const Transform &trans, const GlobalObjectParam &param, const mat4 &matModel, vec3 &mi, vec3 &ma, int width, int height, int mx, int my)
{
    vec3 rayPnt;
	vec3 rayDir;

	getPickingRay(trans, param.fov, param.ncp, width, height, mx, my, rayPnt, rayDir);

    vec4 tmpMa = matModel * vec4(ma.x, ma.y, ma.z, 1);
    vec4 tmpMi = matModel * vec4(mi.x, mi.y, mi.z, 1);

    //tmpMa = mat4::translate(o->m_position) * mat4::rotateY(o->m_rotation.y) * mat4::scale(o->m_scale) * tmpMa;
    //tmpMi = mat4::translate(o->m_position) * mat4::rotateY(o->m_rotation.y) * mat4::scale(o->m_scale) * tmpMi;    

    ma = vec3(tmpMa.x, tmpMa.y, tmpMa.z);
    mi = vec3(tmpMi.x, tmpMi.y, tmpMi.z);

    vec3 v0 = vec3(mi.x, mi.y, mi.z);
    vec3 v1 = vec3(ma.x, mi.y, mi.z);
    vec3 v2 = vec3(ma.x, ma.y, mi.z);
    vec3 v3 = vec3(mi.x, ma.y, mi.z);

    vec3 v4 = vec3(mi.x, mi.y, ma.z);
    vec3 v5 = vec3(ma.x, mi.y, ma.z);
    vec3 v6 = vec3(ma.x, ma.y, ma.z);
    vec3 v7 = vec3(mi.x, ma.y, ma.z);

    float t = 0;

    if(intersectCube(rayPnt, rayDir, v0, v1, v2, v3, v4, v5, v6, v7, t))
    {
        return t;
    }

    return -1.0f;
}