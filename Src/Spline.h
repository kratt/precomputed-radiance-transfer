//Author: Sören Pirk
//Date: 22.01.2013

#ifndef SPLINE
#define SPLINE

#include "Misc.h"

class Spline
{
public:
    enum Config
    {
        CATMULL_ROM = 0,
        CUBIC, 
        HERMITE,
        PICEWISE_HERMITE,
        COSINE,
        LINEAR,
        KOCHANEK_BARTEL,
        ROUNDED_CATMULL_ROM,
        BSPLINE
    };
    
    Spline(Config conf = CATMULL_ROM);
   ~Spline();

   void addPoint(const vec3 &v);
   void clear();
   vec3 interpolatedPoint(float t, Config conf = CATMULL_ROM);
   vec3 point(int n) const;
   int numPoints() const;
   void render(Config conf = CATMULL_ROM);

   void bounds(int &p);

   static vec3 linearInterpolation(const vec3 &p0, const vec3 &p1, float t);
   static vec3 catmullRomInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, float t);
   static vec3 roundedCatmullRomInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, 
                                                 float t);
   static vec3 cubicInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, double t);
   static vec3 bSplineInterpolation(const vec3 &p1, const vec3 &p2, const vec3 &p3, const vec3 &p4, double t);

   static vec3 hermiteInterpolation(const vec3 &p0, const vec3 &p1, const vec3 &p2, const vec3 &p3, 
                                       double t, double tension = 0.0, double bias = 0.0);

   static vec3 picewiseHermiteInterpolation(const vec3 &a, const vec3 &b, const vec3 &startTangent, 
                                               const vec3 &endTangent, float t);
   static vec3 kochanekBartelInterpolation(const vec3 &a, const vec3 &b, const vec3 &c, const vec3 &d, 
                                              double t, double tension = 0.0, double bias = 0.0, double continuity = 0.0f);
   vector<vec3> m_points;

   vec3 m_phantomStart;
   vec3 m_phantomEnd;

private:    
    float m_deltaT;
    Config m_config;
};

#endif

