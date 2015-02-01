#include "Button.h"
#include "Texture.h"

Button::Button(int posX, int posY, int width, int height, QString texPath, QString text)
: m_posX(posX),
  m_posY(posY),
  m_width(width),
  m_height(height),
  m_text(text),
  m_mouseClick(false),
  m_color(0, 0, 0, 1)
{
    QImage imgTexNormal(texPath);
    m_texture = new Texture(imgTexNormal);
}

Button::~Button()
{
    delete m_texture;
}

void Button::render()
{   
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
   
    glEnable2D();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_texture->id());

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glColor4f(m_color.x, m_color.y, m_color.z, m_color.w);
    glPushMatrix();
 
    glTranslatef(m_posX, m_posY, 0.0f);
    glScalef(m_width, m_height, 0.0f);

    glBegin(GL_QUADS);   
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(1.0f, 1.0f, 0.0f);                        

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(1.0f, 0.0, 0.0f);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
    glEnd();
    glPopMatrix();      

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);

    glDisable2D();

    renderString(m_text.toAscii(), m_posX, m_posY+50, Vector4(1, 1, 1, 1), GLUT_BITMAP_HELVETICA_10);

    glPopClientAttrib();
    glPopAttrib();
}

void Button::onMouseMove(uint mx, uint my)
{
}

bool Button::onMouseClick(uint mx, uint my)
{
    float x1 = m_posX;
    float x2 = m_posX + m_width;
    float y1 = m_posY;
    float y2 = m_posY + m_height;

    m_mouseClick = false;

    if(mx > x1 && mx < x2)
    {
        if(my > y1 && my < y2)
        {             
            m_mouseClick = true;
            if(m_variable)
                (*m_variable) = true;
        }
    }

    return m_mouseClick;
}

void Button::onMouseRelease()
{
    m_mouseClick = false;
}

void Button::setPosition(uint x, uint y)
{
	m_posX = x;
	m_posY = y;
}

vec2 Button::position()
{
    return vec2(m_posX, m_posY);
}

vec2 Button::dimensions()
{
    return vec2(m_width, m_height);
}

void Button::setVariable(bool *variable)
{
    m_variable = variable;
}

void Button::setColor(vec4 color)
{
    m_color = color;
}