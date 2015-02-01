//Author: Sören Pirk
//Date: 22.01.2013

#ifndef FRAMEBUFFEROBJECT
#define FRAMEBUFFEROBJECT

#include "Misc.h"

#define FRAMEBUFFER_DEBUG

class FrameBufferObject
{
public:
   FrameBufferObject(GLuint width, GLuint height, GLuint nrTexAtt = 1, GLuint nrBufferAtt = 0, GLboolean attachDepthTex = GL_TRUE, GLint filter = GL_NEAREST);
   ~FrameBufferObject();

    GLvoid bind();
    GLvoid guardedBind();
    GLvoid release();

    GLuint id() const;    
    GLuint emptyTexture(GLuint width, GLuint height, GLuint iformat, GLuint format, GLuint type);
    
    GLvoid createTexAttachment(GLenum attachment, GLuint iFormat, GLuint format, GLuint type);
    GLvoid attachTexture(GLuint texID, GLenum attachment = GL_COLOR_ATTACHMENT0_EXT);
    GLvoid attachTexture(GLuint texID, GLenum attachment, GLenum texTarget, GLint mipLevel, GLint zSlice);

    GLvoid createBufferAttachment(GLenum attachment, GLenum bufferFormat);
    GLvoid attachBuffer(GLuint bufferID, GLenum attachment = GL_COLOR_ATTACHMENT0_EXT);
    GLvoid attachDepthTexture();
    GLvoid attachStencilDepthRenderbuffer();
    GLvoid attachStencilRenderbuffer();
    GLvoid switchToTexAttachment(GLuint texID, GLenum attachment);

    GLvoid checkDrawBuffers();
    GLuint maxAttachments();

    GLvoid info();

    GLuint texAttachment(GLuint idx) const;
    GLuint bufferAttachment(GLuint idx) const;

    GLboolean checkStatus();
    GLchar *lookupAttachment(GLenum att);


private:
    GLuint m_width;
    GLuint m_height;

    GLuint m_fboID;
    GLint m_savedFboID;

    GLuint m_nrTexAtt;
    GLuint m_nrBufferAtt;

    std::map<GLenum, GLuint> m_texAtts;
    std::map<GLenum, GLuint> m_bufferAtts;

    GLuint m_maxColorAtt;

    GLint m_filter;

};

#endif

