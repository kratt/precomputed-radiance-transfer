//Author: Sören Pirk
//Date: 20.04.2010
//Version: 1.0

#ifndef TEXTURE
#define TEXTURE

#include "Misc.h"

class Texture
{

public:
   Texture();
   Texture(GLsizei w, GLsizei h, GLint iFormat, GLint format, GLint type);
   Texture(GLsizei w, GLsizei h, GLint iFormat, GLint format, GLint type, GLvoid *data);
   Texture(const QImage &img);
   Texture(const QImage &img, GLint magFilter, GLint minFilter, GLfloat anisotrophy, GLboolean createMipmaps);
   Texture(QString path);
   Texture(const QFileInfoList &infoList);
   ~Texture();

   void bind();
   void release();
   void create();

   void setWrapMode(GLint wrap);
   void setEnvMode(GLint envMode);
   void setFilter(GLint minFilter, GLint magFilter);
   void setMaxIsotropy(GLfloat anisotropy);

   void deleteTex();

   void render(GLuint posX, GLuint posY, GLfloat width, GLfloat height);

   GLuint id() const;
   GLuint width() const;
   GLuint height() const;

   void createManualMipMaps(QImage img);
   void loadManualMipMaps(QString fileName);

private:
    GLuint  m_id;
    GLsizei m_width;
    GLsizei m_height;
    GLenum  m_target;
    GLint   m_mipLevel;
    GLint   m_internalFormat;
    GLenum  m_format;
    GLint   m_border;
    GLenum  m_type;
    GLvoid *m_data;
    GLint   m_minFilter;
    GLint   m_magFilter;
    GLint   m_wrap;
    GLint   m_envMode;
    GLboolean m_createMipMaps;
    GLfloat m_maxAnisotropy;
    GLboolean m_manualMipMaps;

    std::vector<QImage> m_manualMM;
};

#endif

