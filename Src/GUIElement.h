//Author: Sören Pirk
//Date: 22.01.2013

#ifndef GUIELEMENT
#define GUIELEMENT

#include "Misc.h"

class GUIElement
{
public:
    GUIElement(){};
    virtual ~GUIElement(){};

   virtual void render() = 0;
   virtual bool onMouseClick(uint mx, uint my) = 0;
   virtual void onMouseMove(uint mx, uint my) = 0;
   virtual void onMouseRelease() = 0;

private:  
    
};

#endif

