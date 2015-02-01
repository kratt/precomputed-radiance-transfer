//Author: Sören Pirk
//Date: 22.01.2013

#include "Frustum.h"
#include <math.h>

#include "Misc.h"

Plane::Plane( vec3 &v1,  vec3 &v2,  vec3 &v3) 
{
	set3Points(v1,v2,v3);
}


Plane::Plane() {}

Plane::~Plane() {}


void Plane::set3Points( vec3 &v1,  vec3 &v2,  vec3 &v3) 
{
	vec3 aux1, aux2;

	aux1 = v1 - v2;
	aux2 = v3 - v2;

    normal = aux2.cross(aux1);

	normal.normalize();
	point = v2;
	d = -(normal.dot(point));
}

void Plane::setNormalAndPoint(vec3 &normal, vec3 &point) 
{
	this->normal = normal;
	this->normal.normalize();
	d = -(this->normal.dot(point));
}

void Plane::setCoefficients(float a, float b, float c, float d) 
{
	// set the normal vector
	normal.set(a,b,c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal.set(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;
}

float Plane::distance(vec3 p) 
{
	return (d + normal.dot(p));
}

void Plane::print() 
{
	printf("Plane(");
    normal.print();
    printf("# %f)",d);
}


Frustum::Frustum()
: m_angle (0.0)
{
}

Frustum::~Frustum()
{
}

void Frustum::setCamInternals(float angle, float ratio, float nearD, float farD) 
{

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (float)tan(angle* math_radians * 0.5) ;
	
    nh = nearD * tang;
	nw = nh * ratio; 

	fh = farD  * tang;
	fw = fh * ratio;
}


void Frustum::setCamDef(vec3 &p, vec3 &l, vec3 &u) 
{
	vec3 dir, nc, fc, X, Y, Z;

	Z = p + (l-p); 
	Z.normalize();

	X = cross(u, Z);
	X.normalize();

	Y = cross(Z, X);

	nc = p - Z * nearD;
	fc = p - Z * farD;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;

	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	pl[TOP].set3Points(ntr, ntl, ftl);
	pl[BOTTOM].set3Points(nbl, nbr, fbr);
	pl[LEFT].set3Points(ntl, nbl, fbl);
	pl[RIGHT].set3Points(nbr, ntr, fbr);
	pl[NEARP].set3Points(ntl, ntr, nbr);
	pl[FARP].set3Points(ftr, ftl, fbl);
}


int Frustum::pointInFrustum(vec3 &p) 
{

	int result = INSIDE;
	for(int i=0; i < 6; i++) {

		if (pl[i].distance(p) < 0)
			return OUTSIDE;
	}
	return(result);

}


int Frustum::sphereInFrustum(vec3 &p, float raio) 
{

	int result = INSIDE;
	float distance;

	for(int i=0; i < 6; i++) {
		distance = pl[i].distance(p);
		if (distance < -raio)
			return OUTSIDE;
		else if (distance < raio)
			result =  INTERSECT;
	}
	return(result);

}

int Frustum::boxInFrustum(vec3 &min, vec3 &max) 
{

	int result = INSIDE;

	for(int i=0; i < 6; i++) 
    {
        vec3 normal = pl[i].normal;

        if(pl[i].distance(getVertexP(normal, min, max)) < 0)
        {
            result = OUTSIDE;            
            return result;
        }
		else if (pl[i].distance(getVertexN(normal, min, max)) < 0)
			result =  INTERSECT;
	}

	return(result);
 }

vec3 Frustum::getVertexP(vec3 &normal,  vec3 &min, vec3 &max) 
{
	vec3 res = min;

	if (normal.x > 0)
		res.x += (max.x - min.x);

	if (normal.y > 0)
		res.y += (max.y - min.y);

	if (normal.z > 0)
		res.z += (max.z - min.z);

	return(res);
}



vec3 Frustum::getVertexN(vec3 &normal,  vec3 &min, vec3 &max) 
{
	vec3 res = min;

	if (normal.x < 0)
		res.x += (max.x - min.x);

	if (normal.y < 0)
		res.y += (max.y - min.y);

	if (normal.z < 0)
		res.z += (max.z - min.z);

	return(res);
}




void Frustum::drawPoints() 
{
	glBegin(GL_POINTS);

		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);

		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);

	glEnd();
}


void Frustum::drawLines() 
{
	glBegin(GL_LINE_LOOP);
	//near plane
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//far plane
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//bottom plane
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//top plane
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	//left plane
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
	glEnd();

	glBegin(GL_LINE_LOOP);
	// right plane
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);

	glEnd();
}


void Frustum::drawPlanes() 
{    
	glBegin(GL_QUADS);	
	glColor4f(0.3, 0.3, 0.3, 0.5);
    //glColor4f(1.0, 0.0, 0.0, 1.0);
	//near plane
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);

	//far plane
		//glVertex3f(ftr.x,ftr.y,ftr.z);
		//glVertex3f(ftl.x,ftl.y,ftl.z);
		//glVertex3f(fbl.x,fbl.y,fbl.z);
		//glVertex3f(fbr.x,fbr.y,fbr.z);

    //glColor4f(0.0, 1.0, 0.0, 1.0);

	//bottom plane
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);

	//top plane
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);

        //glColor4f(0.0, 0.0, 1.0, 1.0);

	//left plane

		glVertex3f(ntl.x,ntl.y,ntl.z);
		glVertex3f(nbl.x,nbl.y,nbl.z);
		glVertex3f(fbl.x,fbl.y,fbl.z);
		glVertex3f(ftl.x,ftl.y,ftl.z);


	// right plane
		glVertex3f(nbr.x,nbr.y,nbr.z);
		glVertex3f(ntr.x,ntr.y,ntr.z);
		glVertex3f(ftr.x,ftr.y,ftr.z);
		glVertex3f(fbr.x,fbr.y,fbr.z);

	glEnd();
}

void Frustum::drawNormals() 
{
	vec3 a,b;

	glBegin(GL_LINES);

		// near
		a = (ntr + ntl + nbr + nbl) * 0.25;
		b = a + pl[NEARP].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);

		// far
		a = (ftr + ftl + fbr + fbl) * 0.25;
		b = a + pl[FARP].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);

		// left
		a = (ftl + fbl + nbl + ntl) * 0.25;
		b = a + pl[LEFT].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		
		// right
		a = (ftr + nbr + fbr + ntr) * 0.25;
		b = a + pl[RIGHT].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		
		// top
		a = (ftr + ftl + ntr + ntl) * 0.25;
		b = a + pl[TOP].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);
		
		// bottom
		a = (fbr + fbl + nbr + nbl) * 0.25;
		b = a + pl[BOTTOM].normal;
		glVertex3f(a.x,a.y,a.z);
		glVertex3f(b.x,b.y,b.z);

	glEnd();
}