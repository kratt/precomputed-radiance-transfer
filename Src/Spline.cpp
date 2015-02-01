//Author: Sören Pirk
//Date: 22.01.2013

#include "Spline.h"

Spline::Spline(Config conf) 
: m_config(conf),
  m_deltaT(0.0f),
  m_phantomStart(),
  m_phantomEnd()
{
}

Spline::~Spline()
{
}

void Spline::addPoint(const vec3 &v)
{
    m_points.push_back(v);
    m_deltaT = (float)1 / ((float)m_points.size()-1);

    if(m_points.size() >= 3)
    {
        vec3 sStart = m_points[0];
        vec3 tStart = m_points[1];

        vec3 nStart = vec3(sStart - tStart);
        float lenStart = nStart.length();
        m_phantomStart = sStart +  nStart.normalized() * lenStart * 0.5;

        unsigned int size = m_points.size() - 1;
        vec3 sEnd = m_points[size];
        vec3 tEnd = m_points[size - 1];

        vec3 nEnd = vec3(sEnd - tEnd);
        float lenEnd = nEnd.length();
        m_phantomEnd = sEnd +  nEnd.normalized() * lenEnd * 0.5;
    }
}

void Spline::bounds(int &p)
{
    if (p < 0) 
        p = 0; 

    else if (p >= (int)m_points.size()-1) 
        p = m_points.size() - 1;
}

vec3 Spline::interpolatedPoint(float t, Config conf)
{    
    // Find out in which interval we are on the spline
    int p = (int)(t / m_deltaT);

    int p0 = p - 1; 
    int p1 = p;  
    int p2 = p + 1;
    int p3 = p + 2; 

    vec3 c1;
    if(p0 < 0)
        c1 = m_phantomStart;
    else
        c1 = m_points[p0];

    vec3 v1 = m_points[p1];
    vec3 v2;
    if((uint)p2 >= m_points.size())
        v2 = m_points[m_points.size()-1];    
    else
        v2 = m_points[p2];

    vec3 c2;
    if((uint)p3 >= m_points.size()-1)
        c2 = m_phantomEnd;
    else
        c2 = m_points[p3];

    // Relative (local) time 
	float lt = (t - m_deltaT * (float)p) / m_deltaT;

    if(conf == CATMULL_ROM)
        return Spline::catmullRomInterpolation(c1, v1, v2, c2, lt);
    if(conf == CUBIC)
        return Spline::bSplineInterpolation(c1, v1, v2, c2, lt);
    if(conf == BSPLINE)
        return Spline::cubicInterpolation(c1, v1, v2, c2, lt);
    if(conf == HERMITE)
        return Spline::hermiteInterpolation(c1, v1, v2, c2, lt);
    if(conf == KOCHANEK_BARTEL)
        return Spline::kochanekBartelInterpolation(c1, v1, v2, c2, lt);
    if(conf == ROUNDED_CATMULL_ROM)
        return Spline::roundedCatmullRomInterpolation(c1, v1, v2, c2, lt);

	//return LINEAR in default case
	return Spline::linearInterpolation(v1, v2, lt);
}

vec3 Spline::point(int n) const
{
    return m_points[n];
}

int Spline::numPoints() const
{
    return m_points.size();
}

void Spline::render(Config conf)
{
    glPushMatrix();

        //Interpolated Points
        glColor3f(0.7f, 0.7f, 0.7f);
        glPointSize(1.0f);
        glBegin(GL_POINTS);

        for(float f=0; f<1.0f; f+= 0.005f/m_points.size())
        {
            vec3 v = interpolatedPoint(f, conf);
            glVertex3f(v.x, v.y, v.z);
        }        
        glEnd();

        //Base Points
        glColor3f(1.0f, 1.0f, 1.0f);
        glPointSize(2.0f);
        glBegin(GL_POINTS);

        for(uint i=0; i<m_points.size(); ++i)
        {
            vec3 v = m_points[i];
            glVertex3f(v.x, v.y, v.z);
        }        
        glEnd();


        //Phantom Points
        vec3 p1 = m_phantomStart;
        vec3 p2 = m_phantomEnd;

        glPointSize(3.0f);
        glBegin(GL_POINTS);        
            glColor3f(1.0f, 1.0f, 0.0f);    
            glVertex3f(p1.x, p1.y, p1.z);
            glColor3f(0.0f, 1.0f, 1.0f);                    
            glVertex3f(p2.x, p2.y, p2.z);
        glEnd();

        //Start/End for Phoantom Points
        uint size = m_points.size() - 1;
        vec3 sStart = m_points[0];
        vec3 tStart = m_points[1];
        vec3 sEnd   = m_points[size];
        vec3 tEnd   = m_points[size-1];   

        glPointSize(8.0f);
        glBegin(GL_POINTS);
            glColor3f(1.0f, 0.0f, 0.0f);    
            glVertex3f(sStart.x, sStart.y, sStart.z);
            glColor3f(0.5f, 0.0f, 0.0f);    
            glVertex3f(tStart.x, tStart.y, tStart.z);

            glColor3f(0.0f, 0.0f, 1.0f);    
            glVertex3f(sEnd.x, sEnd.y, sEnd.z);
            glColor3f(0.0f, 0.0f, 0.5f);    
            glVertex3f(tEnd.x, tEnd.y, tEnd.z);
        glEnd();
    glPopMatrix();
}

void Spline::clear()
{
    m_points.clear();
}

vec3 Spline::linearInterpolation(const vec3 &p0, const vec3 &p1, float t)
{
    return p0 * t + (p1 * (1-t));
}

vec3 Spline::catmullRomInterpolation(const vec3 &p0, const vec3 &p1, const Vector3 &p2, const vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    float b1 = 0.5f * (  -t3 + 2*t2 - t);
    float b2 = 0.5f * ( 3*t3 - 5*t2 + 2);
    float b3 = 0.5f * (-3*t3 + 4*t2 + t);
    float b4 = 0.5f * (   t3 -   t2    );

    return (p0*b1 + p1*b2 + p2*b3 + p3*b4); 
}

//not working right
vec3 Spline::roundedCatmullRomInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, 
                                               float t)
{
    vec3 ab = p0 - p1;
    vec3 cb = p2 - p3;

    vec3 bVelo = cb.normalized() - ab.normalized();
    bVelo.normalize();

    vec3 dc = p3 - p2;
    vec3 bc = -cb;   

    vec3 cVelo = dc.normalized() - bc.normalized();
    cVelo.normalize();

    float cbDist = cb.length();

    return Spline::catmullRomInterpolation(bVelo * cbDist, p1, p2, cVelo * cbDist, t);
}

vec3 Spline::cubicInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, double t)
{
   double t2;
   vec3 a0, a1, a2, a3;

   t2 = t*t;
   a0 = p3 - p2 - p0 + p1;
   a1 = p0 - p1 - a0;
   a2 = p2 - p0;
   a3 = p1;

   return(a0*t*t2 + a1*t2+a2*t + a3);
}

//not working right
vec3 Spline::bSplineInterpolation(const vec3 &p1, const vec3 &p2, const vec3 &p3, const vec3 &p4, double t)
{
    Vector4 a, b, c;
    
    a.x = (-p1.x + 3 * p2.x - 3 * p3.x + p4.x) / 6.0;
    a.y = (3 * p1.x - 6 * p2.x + 3 * p3.x) / 6.0;
    a.z = (-3 * p1.x + 3 * p3.x) / 6.0;
    a.w = (p1.x + 4 * p2.x + p3.x) / 6.0;
    
    b.x = (-p1.y + 3 * p2.y - 3 * p3.y + p4.y) / 6.0;
    b.y = (3 * p1.y - 6 * p2.y + 3 * p3.y) / 6.0;
    b.z = (-3 * p1.y + 3 * p3.y) / 6.0;
    b.w = (p1.y + 4 * p2.y + p3.y) / 6.0;

    c.x = (-p1.y + 3 * p2.y - 3 * p3.y + p4.y) / 6.0;
    c.y = (3 * p1.y - 6 * p2.y + 3 * p3.y) / 6.0;
    c.z = (-3 * p1.y + 3 * p3.y) / 6.0;
    c.w = (p1.y + 4 * p2.y + p3.y) / 6.0;

    vec3 p = vec3( ((a.z + t * (a.y + t * a.x))*t+a.w), ((b.z + t * (b.y + t * b.x))*t+b.w), ((c.z + t * (c.y + t * c.x))*t+c.w) ) ;
    
    return p;

}

vec3 Spline::hermiteInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, 
                                     double t, double tension, double bias)
{
    vec3 m0, m1;
    double t2,t3;
    double a0, a1, a2, a3;

    t2 = t * t;
    t3 = t2 * t;

    m0  = (p1-p0)*(1+bias)*(1-tension)/2;
    m0 += (p2-p1)*(1-bias)*(1-tension)/2;
    m1  = (p2-p1)*(1+bias)*(1-tension)/2;
    m1 += (p3-p2)*(1-bias)*(1-tension)/2;
    
    a0 =  2*t3 - 3*t2 + 1;
    a1 =    t3 - 2*t2 + t;
    a2 =    t3 -   t2;
    a3 = -2*t3 + 3*t2;

    return(a0*p1 + a1*m0 + a2*m1 + a3*p2);
}

vec3 Spline::picewiseHermiteInterpolation(const vec3 &a, const vec3 &b, const vec3 &startTangent, 
                                             const vec3 &endTangent, float t)
{
    double t2, t3;
    double a0, a1, b0, b1;

    t2 = t*t;
    t3 = t2*t;

    a0 = (t3 * 2.0f) - (3.0f * t2) + 1.0f;
    a1 = (-2.0f * t3) + (3.0f * t2);    
    b0 = t3 - (2.0f * t2) + t;
    b1 = t3 -   t2;

    return (a0*a + a1*b + b0*startTangent + b1*endTangent);
}

//not working right
vec3 Spline::kochanekBartelInterpolation(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, 
                                                   double t, double tension, double bias, double continuity)
{
    vec3 ab = vec3(b-a).normalized();
    vec3 cd = vec3(d-c).normalized();

    vec3 inTangent = ((1.0f - tension) * (1.0f - continuity) * (1.0f + bias)) * 0.5f * ab 
                      + ((1.0f - tension) * (1.0f + continuity) * (1.0f - bias)) * 0.5f * cd;

    vec3 outTangent = ((1.0f - tension) * (1.0f + continuity) * (1.0f + bias)) * 0.5f * ab
                       + ((1.0f - tension) * (1.0f - continuity) * (1.0f - bias)) * 0.5f * cd;


    //return picewiseHermiteInterpolation(b, c, inTangent, outTangent, t);
    return hermiteInterpolation(inTangent, b, c, outTangent, t);
}