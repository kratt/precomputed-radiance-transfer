//Author: Sören Pirk
//Date: 22.01.2013

#ifndef GLWIDGET
#define GLWIDGET

#include "Misc.h"

class Shader;
class Camera;
class Scene;
class Renderer;
class GUI;
class CameraManager;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
   GLWidget(QGLContext *context, int width, int height);
   ~GLWidget();
   
   void initializeGL();   
   void paintGL();
   void resizeGL(int width, int height);   
   void wheelEvent(QWheelEvent *event);
   void keyPressEvent(QKeyEvent *event);
   void keyReleaseEvent(QKeyEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);   
   void initParams();

private:       
    QPoint m_mouse;

    bool m_leftButton;
    bool m_rightButton;    
	bool m_ctrlPressed;
	bool m_altPressed;
	bool m_shiftPressed;    
    bool m_noOtherKey;

    int m_width;
    int m_height;

	Scene *m_scene;
	Renderer *m_renderer;
	GUI *m_gui;
	CameraManager *m_cameraManager;

	GlobalObjectParam m_param;
    Transform m_trans;

    DWORD m_oldTime;
};

#endif

