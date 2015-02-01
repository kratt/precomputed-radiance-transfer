//Author: Sören Pirk
//Date: 22.01.2013

#ifndef NICEGRID
#define NICEGRID

#include "Misc.h"

class Shader;
class Texture;
class VertexBufferObjectAttribs;

class NiceGrid
{
public:
   NiceGrid(GLfloat size, GLfloat rep);
   ~NiceGrid();

   void render(const Transform &trans, const GlobalObjectParam &param);
   void setDiffuseColor(float r, float g, float b);
   void setAmbientColor(float r, float g, float b);  

private:
    void createVBO();


private:
    GLuint  m_texID;
    GLfloat m_size;
    GLfloat m_rep;

    Shader *m_shader;
    Texture *m_texture;
    VertexBufferObjectAttribs *m_vbo;

    vec3 m_ambient;
    vec3 m_diffuse;

    vec3 m_position;

    float m_backFaceAlpha;
};

#endif

