//Author: Sören Pirk
//Date: 22.01.2013

#ifndef MESH
#define MESH

#include "Misc.h"

class VertexBufferObjectAttribs;

class Mesh
{
public:
   Mesh();
   ~Mesh();

   static vector<VertexBufferObjectAttribs *> Mesh::obj(const QString &fileName, const vec3 &rot = vec3(0.0f, 0.0f, 0.0f), const vec3 &scale = vec3(1.0f, 1.0f, 1.0f), GLenum primitive = GL_TRIANGLES);
   static VertexBufferObjectAttribs *quadLines(int startX, int startY, int width, int height, const vec4 &color = vec4());
   static VertexBufferObjectAttribs *quad(int startX, int startY, int width, int height, const vec4 &color = vec4(), GLenum primitive = GL_QUADS);
   static VertexBufferObjectAttribs *quad(int width, int height, const vec4 &color = vec4(), GLenum primitive = GL_QUADS);
   static VertexBufferObjectAttribs *sphere(float radius, int iterations, const vec4 &color = vec4(), GLenum primitive = GL_TRIANGLES);
   static VertexBufferObjectAttribs *box(const vec3 &mi, const vec3 &ma, const vec4 &color = vec4(), GLenum primitive = GL_QUADS);

private:
    typedef struct 
    {
       double x,y,z;
    } XYZ;
    
    typedef struct 
    {
       vec3 p1,p2,p3;
    } FACET3;


    //Paul Bourke Sphere http://paulbourke.net/miscellaneous/sphere_cylinder/
    static int createNSphere(FACET3 *f, int iterations);
    static int CreateUnitSphere(FACET3 *facets, int iterations);
    static vec3 MidPoint(vec3 p1, vec3 p2);

};

#endif

