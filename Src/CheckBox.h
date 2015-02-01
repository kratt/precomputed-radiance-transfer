//Author: Sören Pirk
//Date: 22.01.2013

#ifndef CHECKBOX
#define CHECKBOX

#include "Misc.h"
#include "GUIElement.h"

class CheckBox : public GUIElement
{
public:
   CheckBox(uint px, uint py, uint w, uint h, QString text);
   ~CheckBox();
   
   void render();
   bool onMouseClick(uint mx, uint my);
   void onMouseMove(uint mx, uint my);
   void onMouseRelease();        
   void setPosition(uint px, uint py);

   void setState(bool value);   
   void setColor(const vec4 &color);
   void setVariable(bool *variable);

   bool state() const;

   vec2 position();
   vec2 dimensions();

private:  
    uint m_width;
    uint m_height;
    uint m_posX;
    uint m_posY;

    QString m_text;

    vec4 m_color;

    bool m_mouseClick;

    bool m_state;
    bool *m_variable;
};

#endif

