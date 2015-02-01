//Author: Sören Pirk
//Date: 22.01.2013

#ifndef GRAPHICALUSERINTERFACE
#define GRAPHICALUSERINTERFACE

#include "Misc.h"
#include "RenderString.h"

class CameraManager;
class VertexBufferObjectAttribs;
class Shader;
class GUIElement;
class Scene;

class GUI : QObject
{
	Q_OBJECT 

public:
   GUI(GlobalObjectParam *param, CameraManager *cameraManager, Scene *m_scene);
   ~GUI();

   void render(const GlobalObjectParam &param);
   void toggleMode();
   int currentMode();
   void setFontColor(const vec4 &color);
   void resize(int width, int height);

   bool onMouseClick(uint mx, uint my);
   void onMouseMove(uint mx, uint my);
   void onMouseRelease(); 

private:
	void initShortcuts();
	void initUIElements();

private slots:
	void onTimer();

private:

	uint m_width;
	uint m_height;

	std::vector<QString> m_shortcuts;

	QTimer m_timer;

	uint m_fpsCount;
	uint m_fps;

	uint m_mode;

    HPTimer m_hpTimer;
    double m_oldTime;

    CameraManager *m_cameraManager;  
	Scene *m_scene;

	vec4 m_fontColor;

	VertexBufferObjectAttribs *m_vboQuad;
	Shader *m_shader;

	vector<GUIElement*> m_guiElements;

	GlobalObjectParam *m_param;
};

#endif

