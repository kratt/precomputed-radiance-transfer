#ifndef BUTTON
#define BUTTON

#include "Misc.h"
#include "GUIElement.h"

class Texture;

class Button : public GUIElement
{
public:
    Button(int posX, int posY, int width, int height, QString texPath, QString text);
    ~Button();

   void render();
   bool onMouseClick(uint mx, uint my);
   void onMouseMove(uint mx, uint my);
   void onMouseRelease();

   void setPosition(uint x, uint y);
   vec2 position();
   vec2 dimensions();

   void setVariable(bool *variable);
   void setColor(vec4 color);

private:
    int m_width;
    int m_height;
    int m_posX;
    int m_posY;

    Texture *m_texture;
   
    QString m_text;

    bool m_mouseClick;

    bool *m_variable;
    vec4 m_color;
};

#endif