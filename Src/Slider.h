//Author: Sören Pirk
//Date: 22.01.2013

#ifndef SLIDER
#define SLIDER

#include "Misc.h"
#include "GUIElement.h"

template <class T> class Slider : public GUIElement
{
public:
   Slider(uint px, uint py, uint w, uint h, QString text);
   ~Slider();

   void render();
   bool onMouseClick(uint mx, uint my);
   void onMouseMove(uint mx, uint my);
   void onMouseRelease();
   void setPosition(uint px, uint py);

   void setValue(T value);   
   void setRange(T min, T max);
   void setColor(const vec4 &color);
   void setVariable(T *variable);
   void setPos(int x, int y);

   T value() const;

   vec2 position();
   vec2 dimensions();

private:
    uint m_width;
    uint m_height;
    uint m_posX;
    uint m_posY;

    QString m_text;

    float m_percent;
    float m_range;
    T m_valueRange;
    T m_value;
    T m_minValue;
    T m_maxValue;
	vec4 m_color;

    bool m_mouseClick;

    T *m_variable;   
};


template <class T>
Slider<T>::Slider(uint px, uint py, uint w, uint h, QString text)
: m_width(w), 
  m_height(h),
  m_posX(px),
  m_posY(py),
  m_mouseClick(false),
  m_text(text),
  m_minValue(static_cast<T>(0.0f)),
  m_maxValue(static_cast<T>(1.0f)),
  m_value(static_cast<T>(0.0f)),
  m_valueRange(static_cast<T>(0.0f)),
  m_variable(NULL),
  m_color(1.0f, 1.0f, 1.0f, 1.0f)
{
	m_range = m_width;
	m_percent = 1.0f;

	m_valueRange = m_maxValue - m_minValue;
}

template <class T>
Slider<T>::~Slider(){}

template <class T>
void Slider<T>::render()
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

    float qx1 = m_posX + 1;
    float qx2 = m_posX + 1 + (m_width-3) * m_percent;
    float qy1 = m_posY + 2;
    float qy2 = m_posY + m_height - 1 ;    
  
    glBegin(GL_QUADS);
        glVertex2f(qx1, qy1);
        glVertex2f(qx1, qy2);
        glVertex2f(qx2, qy2);
        glVertex2f(qx2, qy1);
    glEnd();

    glDisable2D();

    QString sliderText = m_text;

    //sliderText += QString::number(m_value, 'f', 7);
	sliderText += QString::number(m_value);
    renderString(sliderText.toAscii(), m_posX-2, m_posY-5, m_color.x, m_color.y, m_color.z, m_color.w, GLUT_BITMAP_HELVETICA_12);
}

template <class T> 
void Slider<T>::onMouseMove(uint mx, uint my)
{
    float x1 = m_posX;
    float x2 = m_posX + m_width;

    if(m_mouseClick)
    {
        int curPosX = 0;    

        if(mx > x1 && mx < x2)
        {
            curPosX = mx - m_posX;
            m_percent = (float)curPosX / m_range;
        }

        if(mx < x1)
        {
            m_percent = 0.0f;
        }
        if(mx > x2)
        {
            m_percent = 1.0f;
        }

        m_value = static_cast<T>( m_minValue + m_valueRange * m_percent );
		//m_value = static_cast<T>( 1024* int(100 * m_percent) );
		//if(m_value < m_minValue) m_value = m_minValue;
        
        if(m_variable)
            *m_variable = m_value;
    }
}

template <class T>
bool Slider<T>::onMouseClick(uint mx, uint my)
{
    float x1 = m_posX;
    float x2 = m_posX + m_width;
    float y1 = m_posY;
    float y2 = m_posY + m_height;

    m_mouseClick = false;

    int curPosX = 0;

    if(mx > x1 && mx < x2)
    {
        if(my > y1 && my < y2)
        {    
            m_mouseClick = true;

            curPosX = mx - m_posX;
            m_percent = (float)curPosX / m_range;

            m_value = static_cast<T>( m_minValue + m_valueRange * m_percent );
            
            if(m_variable)
                *m_variable = m_value;
        }
    }

    return m_mouseClick;
}

template <class T>
void Slider<T>::onMouseRelease()
{
    m_mouseClick = false;
}

template <class T>
void Slider<T>::setRange(T min, T max)
{
    if(min < max && max > static_cast<T>(0.0f))
    {
        m_minValue = min;
        m_maxValue = max;   

        m_valueRange = max - min;
        m_value = min + m_valueRange * m_percent;

        if(m_variable)
            *m_variable = m_value;
    }
    else
    {
        float tmpMin = static_cast<float>(min);
        float tmpMax = static_cast<float>(max);

        if(tmpMin < tmpMax)
        {
            m_minValue = min;
            m_maxValue = max;        
            
            m_valueRange = static_cast<T>( tmpMax - tmpMin );
            m_value      = static_cast<T>( m_minValue + m_valueRange * m_percent );

            if(m_variable)
                *m_variable = m_value;
        }
        else
        {
            m_minValue = max;
            m_maxValue = min;

            m_valueRange = static_cast<T>( tmpMin - tmpMax );
            m_value      = static_cast<T>( m_minValue + m_valueRange * m_percent );
            
            if(m_variable)
                *m_variable = m_value;
        }
    }
}

template <class T>
T Slider<T>::value() const
{
    return m_value;
}

template <class T>
void Slider<T>::setValue(T value)
{
    m_value = value;

    if(m_value < m_minValue)
        m_value = m_minValue;

    if(m_value > m_maxValue)
        m_value = m_maxValue;

    if(m_variable)
        *m_variable = value;

    if(value < static_cast<T>(0.0f))
	{
        m_percent = (abs(static_cast<float>(m_minValue)) - fabs(static_cast<float>(m_value))) / static_cast<float>(m_valueRange);
	} else {
        m_percent = fabs(static_cast<float>(m_value) - static_cast<float>(m_minValue)) / static_cast<float>(m_valueRange);
	}
}

template <class T>
void Slider<T>::setColor(const vec4 &color)
{
    m_color = color;
}

template <class T>
void Slider<T>::setVariable(T *variable)
{
    m_variable = variable;
}

template <class T>
void Slider<T>::setPosition(uint x, uint y)
{
	m_posX = x;
	m_posY = y;
}

template <class T>
vec2 Slider<T>::position()
{
	return vec2(m_posX, m_posY);
}

template <class T>
vec2 Slider<T>::dimensions()
{
	return vec2(m_width, m_height);
}

#endif

