//Author: Sören Pirk
//Date: 17.02.2014

#ifndef RENDERSTRING
#define RENDERSTRING

#include "Misc.h"

class Texture;

class RenderString
{
private:
    QString m_string;
    int m_posX;
    int m_posY;
    int m_size;
    vec4 m_color;
    int m_width;
    int m_height;

    Texture *m_tex;

public:
    RenderString(QString string, int size, int posX, int posY, const vec4 &color);
    ~RenderString();

    void init();
    void render();
};

#endif