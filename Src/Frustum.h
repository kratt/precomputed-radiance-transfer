//Author: Sören Pirk
//Date: 22.01.2013

#ifndef FRUSTUM
#define FRUSTUM

#include "Misc.h"

class Plane  
{

public:

	vec3 normal, point;
	float d;

	Plane( vec3 &v1,  vec3 &v2,  vec3 &v3);
	Plane(void);
	~Plane();

	void set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3);
	void setNormalAndPoint(vec3 &normal, vec3 &point);
	void setCoefficients(float a, float b, float c, float d);
	float distance(vec3 p);

	void print();
};

class Frustum 
{
private:

	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

    float m_angle;

public:

	static enum {OUTSIDE, INTERSECT, INSIDE};

	Plane pl[6];


	vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr;
	float nearD, farD, ratio, angle,tang;
	float nw,nh,fw,fh;

	Frustum();
	~Frustum();

	void setCamInternals(float angle, float ratio, float nearD, float farD);
	void setCamDef(vec3 &p, vec3 &l, vec3 &u);
	int pointInFrustum(vec3 &p);
	int sphereInFrustum(vec3 &p, float raio);
	//int boxInFrustum(AABox &b);
    int boxInFrustum(vec3 &min, vec3 &max);

    vec3 getVertexN(vec3 &normal, vec3 &min, vec3 &max);
    vec3 getVertexP(vec3 &normal, vec3 &min, vec3 &max);

	void drawPoints();
	void drawLines();
	void drawPlanes();
	void drawNormals();
};


#endif