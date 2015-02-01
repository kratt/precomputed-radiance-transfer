//Author: Sören Pirk
//Date: 22.01.2013

#include "FrameBufferObjectMultisample.h"

FrameBufferObjectMultisample::FrameBufferObjectMultisample(GLuint width, GLuint height, GLuint samples)
: m_width(width),
  m_height(height),
  m_filterParam(GL_NEAREST)
{
    GLsizei maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
    qDebug() <<"FRAMEBUFFERMULTISAMPLE::Samples: " << samples;
    qDebug() <<"FRAMEBUFFERMULTISAMPLE::MaxSamples: " << maxSamples;

    //Color Renderbuffer
	glGenRenderbuffers(1, &m_colorRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_colorRenderBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_RGBA, width, height);

    //Depth Renderbuffer
	glGenRenderbuffers(1, &m_depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT32F , width, height);

    //Framebuffer with 2 Renderbuffer Attachment
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



    //Texture
    glGenTextures(1, &m_texColor);
	glBindTexture(GL_TEXTURE_2D, m_texColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterParam);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, 0);

    //Depth Texture
    glGenTextures(1, &m_texDepth);
	glBindTexture(GL_TEXTURE_2D, m_texDepth);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterParam);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterParam);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //Framebuffer with 2 Texture Attachment
	glGenFramebuffers(1, &m_fboResolve);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fboResolve);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColor, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texDepth, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

    qDebug() << "FRAMEBUFFERMULTISAMPLE::Size:" << width << "x" << height;
}

FrameBufferObjectMultisample::~FrameBufferObjectMultisample()
{
    glDeleteTextures(1, &m_texDepth);
    glDeleteTextures(1, &m_texColor);

    glDeleteRenderbuffers(1, &m_colorRenderBuffer);
    glDeleteRenderbuffers(1, &m_depthRenderBuffer);

    glDeleteFramebuffers(1, &m_fbo);
    glDeleteFramebuffers(1, &m_fboResolve);

    m_texDepth = 0;
    m_texColor = 0;

    m_colorRenderBuffer = 0;
    m_depthRenderBuffer = 0;

    m_fbo = 0;
    m_fboResolve = 0;

    qDebug() << "FRAMEBUFFERMULTISAMPLE::deleted";
}

void FrameBufferObjectMultisample::bind()
{
    glEnable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FrameBufferObjectMultisample::release()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_MULTISAMPLE);   
}

void FrameBufferObjectMultisample::blit()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboResolve);
	glBlitFramebuffer(0, 0, m_width, m_height,  0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, m_filterParam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObjectMultisample::blitDepth()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboResolve);
	glBlitFramebuffer(0, 0, m_width, m_height,  0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, m_filterParam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObjectMultisample::blitColor()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboResolve);
	glBlitFramebuffer(0, 0, m_width, m_height,  0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, m_filterParam);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBufferObjectMultisample::depthTex()
{
    return m_texDepth;
}

GLuint FrameBufferObjectMultisample::colorTex()
{
    return m_texColor;
}

void FrameBufferObjectMultisample::setDepthFromFBO(FrameBufferObjectMultisample *fbo)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo->fboRender());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->fboRender());

	glBlitFramebuffer(0, 0, m_width, m_height,  0, 0, m_width, m_height, GL_DEPTH_BUFFER_BIT, m_filterParam);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

}

GLuint FrameBufferObjectMultisample::fboRender()
{
	return m_fbo;
}

GLuint FrameBufferObjectMultisample::fboTexture()
{
	return m_fboResolve;
}