//Author: Sören Pirk
//Date: 22.01.2013

#ifndef FRAMEBUFFEROBJECTMULTISAMPLE
#define FRAMEBUFFEROBJECTMULTISAMPLE

#include "Misc.h"

class FrameBufferObjectMultisample
{
public:
    FrameBufferObjectMultisample(GLuint width, GLuint height, GLuint samples);
    ~FrameBufferObjectMultisample();
    void bind();
    void release();
    void blitDepth();
	void blitColor();
	void blit();

	void setDepthFromFBO(FrameBufferObjectMultisample *fbo);

    GLuint depthTex();
    GLuint colorTex();

	GLuint fboRender();
	GLuint fboTexture();

private:
    GLuint m_colorRenderBuffer;
    GLuint m_depthRenderBuffer;
    GLuint m_fbo;
    GLuint m_fboResolve;

    GLuint m_texColor;
    GLuint m_texDepth;

    GLuint m_width;
    GLuint m_height;

	GLint m_filterParam;
};

#endif

