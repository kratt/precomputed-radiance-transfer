//Author: Sören Pirk
//Date: 05.02.2013

#ifndef COMBOBOX
#define SLIDCOMBOBOXER

#include "Misc.h"
#include "GUIElement.h"

class ComboBox : public GUIElement
{
public:

   ComboBox(uint px, uint py, uint w, uint h, QString text);
   ~ComboBox();
   
   void render();
   bool onMouseClick(uint mx, uint my);
   void onMouseMove(uint mx, uint my);
   void onMouseRelease();
   void setPosition(uint px, uint py);

   void setActiveIdx(int active);   
   void setColor(const vec4 &color);
   void setVariable(int *variable);
   int  activeIdx() const;
   void  addItem(int id, QString name);

   vec2 position();
   vec2 dimensions();

private:  
    uint m_width;
    uint m_height;
    uint m_posX;
    uint m_posY;

    QString m_text;
    vec4 m_color;

    bool m_unfold;
    bool m_mouseClick;
    int *m_variable;   
    int m_activeIdx;
    QString m_activeText;
    int rowHeight;
    int m_hooverId;

    QMap<int, QString> m_items;

};

#endif

