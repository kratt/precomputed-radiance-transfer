//Author: Sören Pirk
//Date: 05.02.2013

#include "ComboBox.h"

ComboBox::ComboBox(uint px, uint py, uint w, uint h, QString text)
: m_width(w), 
  m_height(h),
  m_posX(px),
  m_posY(py),
  m_unfold(false),
  m_mouseClick(false),
  m_text(text),
  m_variable(NULL),
  m_activeIdx(0),
  m_activeText("NULL"),
  rowHeight(20),
  m_hooverId(-1),
  m_color(1.0f, 1.0f, 1.0f, 1.0f)
{    
}

ComboBox::~ComboBox()
{
}

void ComboBox::render()
{
    glEnable2D();

    float x1 = m_posX;
    float x2 = m_posX + m_width;
    float y1 = m_posY;
    float y2 = m_posY + m_height;
    
    glColor4f(m_color.x, m_color.y, m_color.z,  m_color.w);
    
    glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y1);

        glVertex2f(x1-1, y2);
        glVertex2f(x2, y2);
        
        glVertex2f(x1, y1);
        glVertex2f(x1, y2);

        glVertex2f(x2, y1);
        glVertex2f(x2, y2);
    glEnd();


    float qx1 = m_posX + m_width - 20;
    float qx2 = m_posX + m_width - 2;
    float qy1 = m_posY + 2;
    float qy2 = m_posY + m_height - 1 ;    
  
    glBegin(GL_QUADS);
        glVertex2f(qx1, qy1);
        glVertex2f(qx1, qy2);
        glVertex2f(qx2, qy2);
        glVertex2f(qx2, qy1);
    glEnd();

    if(m_unfold)
    {
        float sx1 = m_posX;
        float sx2 = m_posX + m_width;
        float sy1 = m_posY + m_height + 3;
        float sy2 = m_posY + m_height + 3 + rowHeight * m_items.size();

        glBegin(GL_LINES);
            glVertex2f(sx1, sy1);
            glVertex2f(sx2, sy1);

            glVertex2f(sx1-1, sy2);
            glVertex2f(sx2, sy2);
            
            glVertex2f(sx1, sy1);
            glVertex2f(sx1, sy2);

            glVertex2f(sx2, sy1);
            glVertex2f(sx2, sy2);
        glEnd();

        if(m_hooverId >= 0)
        {
            glColor4f(m_color.x, m_color.y, m_color.z,  m_color.w);
            float hx1 = m_posX+1;
            float hx2 = m_posX + m_width - 2;
            //float hy1 = m_posY + m_height + 2 + 13 + rowHeight * m_hooverId;
            //float hy2 = m_posY + m_height + 2 + 13 + rowHeight * m_hooverId + 10 ;    
            int y = m_posY + m_height + 5+ rowHeight * (m_hooverId);
          
            glBegin(GL_QUADS);
                glVertex2f(hx1, y);
                glVertex2f(hx1, y+rowHeight-3);
                glVertex2f(hx2, y+rowHeight-3);
                glVertex2f(hx2, y);
            glEnd();
        }

        int i = 0;
        for(QMap<int, QString>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            QString text = iter.value(); 

            renderString(text.toAscii(), sx1+5, sy1+rowHeight-5+(rowHeight)*i, m_color.x, m_color.y, m_color.z, m_color.w, GLUT_BITMAP_HELVETICA_12);
            i++;
        }

    }

    glDisable2D();

    QString sliderText = m_activeText;
    renderString(m_text.toAscii(), m_posX-2, m_posY-5, m_color.x, m_color.y, m_color.z, m_color.w, GLUT_BITMAP_HELVETICA_12);
    renderString(sliderText.toAscii(), m_posX+5, m_posY+14, m_color.x, m_color.y, m_color.z, m_color.w, GLUT_BITMAP_HELVETICA_12);
}

void ComboBox::onMouseMove(uint mx, uint my)
{
    m_hooverId = -1;
    if(m_unfold)
    {
        int i=0; 
        for(QMap<int, QString>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
        {
            float sx1 = m_posX;
            float sx2 = m_posX + m_width;
      
            int y = m_posY + m_height + 5+ rowHeight * (i);

            if(mx > sx1 && mx < sx2)
            {
                if(my > uint(y) && my < uint(y+rowHeight))
                {    
                    m_hooverId = i;
                }
            }

            i++;
        }
    }
}

bool ComboBox::onMouseClick(uint mx, uint my)
{
    float qx1 = m_posX + m_width - 20;
    float qx2 = m_posX + m_width - 2;
    float qy1 = m_posY + 2;
    float qy2 = m_posY + m_height - 1 ;  

    m_mouseClick = false;

    if(!m_unfold)
    {
        if(mx > qx1 && mx < qx2)
        {
            if(my > qy1 && my < qy2)
            {    
                m_mouseClick = true;
                m_unfold = true;
            }
        }
    }
    else
    {
        if(mx > qx1 && mx < qx2)
        {
            if(my > qy1 && my < qy2)
            {    
                m_mouseClick = false;
                m_unfold = false;
            }
        }
        else
        {
            int i=0;
            for(QMap<int, QString>::iterator iter = m_items.begin(); iter != m_items.end(); ++iter)
            {
                float sx1 = m_posX;
                float sx2 = m_posX + m_width;
                int y = m_posY + rowHeight + (rowHeight) * i;
                
                if(mx > sx1 && mx < sx2)
                {
                    if(my > uint(y) && my < uint(y+rowHeight))
                    {    
                        m_activeIdx = iter.key();
                        m_activeText = iter.value();
                        m_unfold = false;
                        
                        if(m_variable)
                            *m_variable = m_activeIdx;
                    }
                }

                i++;
            }
        }
    }



    return m_mouseClick;
}

void ComboBox::onMouseRelease()
{
    m_mouseClick = false;
}

int ComboBox::activeIdx() const
{
    return m_activeIdx;
}

void ComboBox::setActiveIdx(int active)
{
    if(m_items.size() == 0)
        return;

    m_activeIdx = active;

    QMap<int, QString>::iterator iter = m_items.find(active);
    if(iter == m_items.end())
    {
        m_activeIdx = 0;
        m_activeText = m_items[m_activeIdx];
    }
    else
    {
        m_activeIdx = iter.key();
        m_activeText = iter.value();
    }

    if(m_variable)
        *m_variable = m_activeIdx;
}

void ComboBox::setColor(const vec4 &color)
{
    m_color = color;
}

void ComboBox::setVariable(int *variable)
{
    m_variable = variable;
}

void ComboBox::setPosition(uint x, uint y)
{
	m_posX = x;
	m_posY = y;
}

void ComboBox::addItem(int id, QString name)
{
    m_items.insert(id, name);
    m_activeIdx = id;
    m_activeText = name;
}

vec2 ComboBox::position()
{
    return vec2(m_posX, m_posY);
}

vec2 ComboBox::dimensions()
{
    return vec2(m_width, m_height);
}