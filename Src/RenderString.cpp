//Author: Sören Pirk
//Date: 17.02.2014

#include "RenderString.h"
#include "Texture.h"

RenderString::RenderString(QString string, int size, int posX, int posY, const vec4 &color)
: m_string(string), 
  m_posX(posX),
  m_posY(posY),
  m_size(size),
  m_color(color),
  m_tex(NULL)
{
    init();
}

RenderString::~RenderString()
{
    delete m_tex;
}

void RenderString::init()
{
    QFont font("Helvetica", m_size) ;
    QFontMetrics fm(font);

    m_width = fm.width(m_string);
    m_height = m_size;//fm.height();

    QImage img(m_width, m_height, QImage::Format_ARGB32);
    img.fill(qRgba(0, 0, 0, 0));

    QPainter p(&img);
    p.setPen(QColor(m_color.x * 255, m_color.y * 255, m_color.z * 255, m_color.w * 255));
    p.setFont(font);
    p.drawText(0, m_height, m_string);
    p.end();

    m_tex = new Texture(img, GL_NEAREST, GL_NEAREST, 1.0f, GL_FALSE);
}

void RenderString::render()
{
    renderTexture(m_tex->id(), m_posX, m_posY, m_width, m_height);
}
