//Author: Sören Pirk
//Date: 22.01.2013

#include "GLWidget.h"
#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "Scene.h"
#include "GUI.h"
#include "CameraManager.h"
#include "Light.h"

GLWidget::GLWidget(QGLContext *context, int width, int height)
: QGLWidget(context),
  m_leftButton(false),
  m_rightButton(false),
  m_width(width),
  m_height(height),
  m_ctrlPressed(false),
  m_altPressed(false),
  m_shiftPressed(false),
  m_noOtherKey(true)
{
    this->resize(m_width, m_height);
    this->setMouseTracking(true);
}

GLWidget::~GLWidget()
{
	delete m_scene;
	delete m_cameraManager;
	delete m_gui;
	delete m_renderer;
}

void GLWidget::initializeGL() 
{
    glewInit();
	
	initParams();

	m_cameraManager = new CameraManager();
    m_scene = new Scene(m_cameraManager);
	m_gui = new GUI(&m_param, m_cameraManager, m_scene);
	m_renderer = new Renderer(m_scene, m_cameraManager, m_gui);    

    glEnable(GL_DEPTH_TEST);     
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);    

    glEnable(GL_SAMPLE_SHADING);
    glMinSampleShading(0.0f);

    //glProvokingVertex(GL_LAST_VERTEX_CONVENTION );
}

void GLWidget::initParams()
{
    m_param.lightPos            = vec3(0.0f, 0.0f, 0.0f);
    m_param.lightDir            = vec3(0.0f, 0.0f, 0.0f);
    m_param.camPos              = vec3(0.0f, 0.0f, 0.0f);
    m_param.applyShadow         = true;
    m_param.gridRenderMode      = 0;
    m_param.shadowIntensity     = 0.0f;

    m_param.shadowMapID         = 0;
    m_param.shadowMapBlurredID  = 0;
    m_param.polygonMode         = 0;

    m_param.windowWidth         = m_width;
    m_param.windowHeight        = m_height;

    m_param.renderMesh          = false;
    m_param.renderObjects       = true;

    m_param.clipPlaneGround     = vec4(0.0f, -1.0f, 0.0f, 4.0f);
    m_param.ncp                 = 0.0f;
    m_param.fcp                 = 0.0f;
    m_param.fov                 = 0.0f;

    m_param.polygonOffsetUnits = 0.0f;
    m_param.polygonOffsetFactor = 0.0f;
    m_param.depthRangeMax       = 1.0f;
    m_param.depthRangeMin       = 0.0f;
}

void GLWidget::paintGL()
{                         
	m_cameraManager->currentPerspective(m_trans);
    m_cameraManager->currentCamParams(m_param);
	
	m_param.windowWidth = this->width();
	m_param.windowHeight = this->height();
    m_param.lightPos = m_scene->m_light->position();

    m_renderer->render(m_trans, m_param);

    DWORD time = GetTickCount();
    float delta = time - m_oldTime;
    if(delta > 25)
    {
        m_scene->update(1.0f/delta);
        m_oldTime = time;
    }

    this->update();
}

void GLWidget::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;

	m_renderer->resize(w, h);
	m_cameraManager->resize(w, h);
    m_gui->resize(w, h);
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if(!m_altPressed && !m_ctrlPressed)
    {
	    m_cameraManager->onMouseWheel(event->delta());
    }

    if(m_altPressed)
    {
		vec3 lPos = m_scene->m_light->position();
	    float delta = lPos.y * 0.1;

        if (event->delta() > 0) 
			m_scene->m_light->setPosition(vec3(lPos.x, lPos.y+delta, lPos.z));
        else 
            m_scene->m_light->setPosition(vec3(lPos.x, lPos.y-delta, lPos.z));
    }

    event->accept();    
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint newMouse(event->x(), event->y());
	QPoint diff = newMouse - m_mouse;

    m_gui->onMouseMove(event->x(), event->x());

    if(!m_altPressed && !m_ctrlPressed)
    {
	    if(m_leftButton)
	    {            		
		    m_cameraManager->onMouseMove(diff.x(), diff.y(), 0);
	    }
	    else if(m_rightButton)
	    {
		    m_cameraManager->onMouseMove(diff.x(), diff.y(), 1);
	    }
    }

    if(m_leftButton && m_altPressed)
    {
        if(m_ctrlPressed)
            m_scene->m_light->recordPath(true);
        else
            m_scene->m_light->recordPath(false);

		m_scene->m_light->move(m_cameraManager, diff.x()*0.1, diff.y()*0.1);
    }

    if(m_leftButton && m_ctrlPressed)
    {

    }

    m_mouse.setX(event->x());
    m_mouse.setY(event->y());    
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if(!m_gui->onMouseClick(event->x(), event->y()))
    {
        if(event->button() == Qt::LeftButton)
            m_leftButton = true; 

        if(event->button() == Qt::RightButton)
            m_rightButton = true;
    }

    if(m_ctrlPressed)
    {

    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_gui->onMouseRelease();

    if(event->button() == Qt::LeftButton)
        m_leftButton = false;   

    if(event->button() == Qt::RightButton)
        m_rightButton = false;
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{   
	m_cameraManager->onKeyPress(event->key());

    switch (event->key()) 
    {
		case Qt::Key_PageUp: 
			m_cameraManager->increaseSpeed();
            break;
        case Qt::Key_PageDown:
			m_cameraManager->decreaseSpeed();
            break;
        case Qt::Key_Left:
            break;
        case Qt::Key_Right:
            break;
		case Qt::Key_Space:
			loop(m_param.polygonMode, 0, 1, 1);
			break;
        case Qt::Key_Plus:
            break;
        case Qt::Key_Minus:
            break;
        case Qt::Key_Enter:
        case Qt::Key_Return:
            break;


        case Qt::Key_A:
            //Camera Strafe Left
            break;
        case Qt::Key_B:
            break;
        case Qt::Key_C:
            break;
        case Qt::Key_D:
            //Camera Strafe Right
            break;
        case Qt::Key_E:
            break;
        case Qt::Key_F:
            break;
        case Qt::Key_G:
            break;
        case Qt::Key_H:
            break;
        case Qt::Key_I:   
            m_cameraManager->toggleInterpolation();
            break;
        case Qt::Key_J:
            break;
        case Qt::Key_K:
            break;
        case Qt::Key_L:
            m_scene->m_light->toggleMode();
            break;
        case Qt::Key_M:
            break;
        case Qt::Key_N:
            break;
        case Qt::Key_O:
            break;
        case Qt::Key_P:
            loop(m_param.gridRenderMode, 0, 3);
            break;
        case Qt::Key_Q:
            break;
        case Qt::Key_U:
            m_param.applyShadow = !m_param.applyShadow;
            break;
        case Qt::Key_R:
            break;
        case Qt::Key_S:
            //Camera Back
            break;
        case Qt::Key_T:
            break;
        case Qt::Key_V:
            break;
        case Qt::Key_W:
            //Camera Forward
            break;
        case Qt::Key_X:
            break;
        case Qt::Key_Y:
            break;
        case Qt::Key_Z:
            break;

        case Qt::Key_F1:
            this->setWindowState(this->windowState() ^ Qt::WindowFullScreen); 
            break;
        case Qt::Key_F2:
			m_cameraManager->toggleCam();
            break;        
        case Qt::Key_F3:            
            saveFrameBuffer(this);
            break;        
        case Qt::Key_F4:
			m_renderer->toggleBGColor();
            break;
        case Qt::Key_F5:
            if(m_noOtherKey)
			    m_gui->toggleMode();

            if(m_ctrlPressed)
                m_cameraManager->toggleFrameset();
            break;
       case Qt::Key_F6:			
            if(m_ctrlPressed)
                m_cameraManager->addFrame();
            break;
       case Qt::Key_F7:			
            if(m_ctrlPressed)
                m_cameraManager->clearFrameset();
            break;
       case Qt::Key_F8:			
            if(m_ctrlPressed)
                m_cameraManager->saveFrameset();
            break;
       case Qt::Key_F9:			
            break;
       case Qt::Key_F10:			
            break;
       case Qt::Key_F11:			
            break;
       case Qt::Key_F12:			
            break;

		case Qt::Key_Control:
			m_ctrlPressed = true;
            m_noOtherKey = false;
			break;
		case Qt::Key_Alt:
			m_altPressed = true;
            m_noOtherKey = false;
			break;
		case Qt::Key_Shift:
			m_shiftPressed = true;
            m_noOtherKey = false;
			break;
        case Qt::Key_Escape:            
            exit(0);
            break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{
	m_cameraManager->onKeyRelease(event->key());

    switch (event->key()) 
    {
		case Qt::Key_Control:
			m_ctrlPressed = false;
            m_noOtherKey = true;
			break;
		case Qt::Key_Alt:
			m_altPressed = false;
            m_noOtherKey = true;
			break;
		case Qt::Key_Shift:
			m_shiftPressed = false;
            m_noOtherKey = true;
			break;
		default:
            break;
    }
}
