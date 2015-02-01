//Author: Sören Pirk
//Date: 22.01.2013

#ifndef VERTEXBUFFEROBJECTATTRIBS
#define VERTEXBUFFEROBJECTATTRIBS

#include "Misc.h"

class VertexBufferObjectAttribs
{
public:
    struct DATA
    {
        float vx, vy, vz, vw;
        float nx, ny, nz, nw;
        float cx, cy, cz, cw;        
        float tx, ty, tz, tw;
    };


public:
    VertexBufferObjectAttribs();
    ~VertexBufferObjectAttribs();

    void bind();
    GLuint id() const;
    void release();
    void render();
    
	GLuint createVBO(GLenum target, GLuint dataSize, const void* data, GLenum usage);
    void setData(const DATA *data, GLenum usage, GLuint nrVertices, GLenum primitiveMode);
    void setIndexData(const GLvoid *data, GLenum usage, GLint nrIndices);
    void addAttrib(GLint attribLoc);
    void setVerticesToRender(GLuint nrVertices); 
    void bindAttribs();
    void setDynamicRendering(GLboolean dynamicRendering);

    GLuint nrVertices() const;
    GLuint nrDynamicVertices() const;


private:
    GLuint m_nrVertices;
    GLuint m_nrDynamicVertices;
    GLuint m_nrIndices;
    GLuint m_nrDynamicIndices;
    GLuint m_sizeBytesVertices;
    GLuint m_sizeBytesIndices;
    GLboolean m_useIndexBuffer;
    
    GLuint m_bufferId;
    GLuint m_indexBufferId;
    
    GLenum m_primitiveMode;
    GLenum m_usage;

    GLuint m_sizeAsStride;

    vector<GLint> m_attribLocations;

    GLboolean m_dynamicRendering;   
};

#endif

