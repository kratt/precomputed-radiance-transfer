//Author: Sören Pirk
//Date: 22.01.2013

#include "Misc.h"
#include "GUI.h"
#include "CameraManager.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Slider.h"
#include "Scene.h"
#include "GUIElement.h"
#include "CheckBox.h"

GUI::GUI(GlobalObjectParam *param, CameraManager *cameraManager, Scene *m_scene)
: m_width(0),
  m_height(0),
  m_fps(0),
  m_fpsCount(0),
  m_mode(0),
  m_oldTime(0),
  m_cameraManager(cameraManager),
  m_scene(m_scene),
  m_fontColor(0.9f, 0.9f, 0.9f, 1.0),
  m_param(param)
{
	initShortcuts();
	initUIElements();

	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_timer.start(200);

    m_hpTimer.reset();

	m_vboQuad = Mesh::quad(0, 0, 1, 1, vec4(0.0f, 0.0f, 0.0f, 0.7f));

	m_shader = new Shader("Shader/GUI.vert.glsl", "Shader/GUI.frag.glsl");
	m_shader->bindAttribLocations();
}

GUI::~GUI()
{
	delete m_vboQuad;
	delete m_shader;
}

void GUI::initShortcuts()
{
	m_shortcuts.push_back(QString("Toggle Fullscreen (F1)"));
	m_shortcuts.push_back(QString("Toggle Cameras (F2)"));
	m_shortcuts.push_back(QString("Screen Shot (F3)"));
	m_shortcuts.push_back(QString("Toggle Background Color (F4)"));
    m_shortcuts.push_back(QString("Toggle Shadow (U)"));
    m_shortcuts.push_back(QString("Adjust Cam Speed (PgUp/PgDown)"));
    m_shortcuts.push_back(QString("Interpolate Cam (I)"));
}

void GUI::initUIElements()
{
    int y = 120;
    
    Slider<float> *s01 = new Slider<float>(10, y, 200, 15, "Shadow Intensity: ");
    s01->setRange(0.0f, 1.0f);
    s01->setVariable(&m_param->shadowIntensity);
    s01->setValue(0.3f);
    m_guiElements.push_back(s01);

    y+=45;
    Slider<float> *s02 = new Slider<float>(10, y, 200, 15, "Offset Units: ");
    s02->setRange(-1000.0f, 1000.0f);
    s02->setVariable(&m_param->polygonOffsetUnits);
    s02->setValue(0.0f);
    m_guiElements.push_back(s02);

    y+=45;
    Slider<float> *s03 = new Slider<float>(10, y, 200, 15, "Offset Factor: ");
    s03->setRange(-1000.0f, 1000.0f);
    s03->setVariable(&m_param->polygonOffsetFactor);
    s03->setValue(0.0f);
    m_guiElements.push_back(s03);


    y+=45;
    Slider<float> *s04 = new Slider<float>(10, y, 200, 15, "Depth Range Max: ");
    s04->setRange(0.0f, 1.0f);
    s04->setVariable(&m_param->depthRangeMax);
    s04->setValue(1.0f);
    m_guiElements.push_back(s04);

    y+=45;
    Slider<float> *s05 = new Slider<float>(10, y, 200, 15, "Depth Range Min: ");
    s05->setRange(0.0f, 1.0f);
    s05->setVariable(&m_param->depthRangeMin);
    s05->setValue(0.0f);
    m_guiElements.push_back(s05);
	
    y+=45;
    CheckBox *c01 = new CheckBox(10, y, 15, 15, "Shadows");
    c01->setVariable(&m_param->applyShadow);
    c01->setState(true);    	
    m_guiElements.push_back(c01);
}

void GUI::render(const GlobalObjectParam &param)
{
	m_width = param.windowWidth;
	m_height = param.windowHeight;

    mat4 model      = mat4::identitiy();
    mat4 view       = mat4::translate(0, 0, -1);
    mat4 projection = mat4::orthographic(0, m_width, m_height, 0, -1, 1);

	Transform trans;
	trans.view = view;
	trans.projection = projection;

    double frameTime = m_hpTimer.time();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);  
	glDisable(GL_MULTISAMPLE);
    glDisable(GL_TEXTURE_2D);

	glLineWidth(0.1f);

	if(m_mode == 0)
	{
		//QString helpStr = QString("(F5) Show GUI ... ");
		//renderString(helpStr.toAscii(), 10, 15, vec4(m_fontColor.x, m_fontColor.y, m_fontColor.z, 1.0f), GLUT_BITMAP_HELVETICA_12);

		//QString fpsStr = QString("FPS: ") + QString::number(m_fps);
        //renderString(fpsStr.toAscii(), m_width - 65, 15, vec4(m_fontColor.x, m_fontColor.y, m_fontColor.z, 1.0f), GLUT_BITMAP_HELVETICA_12);

        //RenderString rStr("TEST", 10, 100, 100, m_fontColor);
        //rStr.render();
	}
	else if(m_mode == 1)
	{
        uint stepX = 250, stepY = 20;

        uint nrX = max<int>(2, m_width / stepX);
        uint nrY = max<int>(2, m_shortcuts.size() / nrX+1);

		uint startX = 10, startY = m_height - (nrY * 18 + 15);
		uint posX = startX, posY = startY;     

		m_shader->bind();
			
			m_shader->setMatrix("matView", view, GL_TRUE);
			m_shader->setMatrix("matProjection", projection, GL_TRUE);

			model = mat4::scale(m_width, 80.0f, 1.0f);
		    m_shader->setMatrix("matModel", model, GL_TRUE); 

			m_vboQuad->render();

			float heightY = (nrY * 18 + 45);
			model = mat4::translate(0.0f, m_height-heightY, 0.0f) * mat4::scale(m_width, heightY, 1.0f);
		    m_shader->setMatrix("matModel", model, GL_TRUE); 

			m_vboQuad->render();

		m_shader->release();

	    for(uint i=0; i<m_guiElements.size(); ++i)
	    {
		    m_guiElements[i]->render();
	    }

        //top
        QString strCamPos = ("LOD CamPos: " + QString::number(param.camPos.x, 'f', 2) + " " + QString::number(param.camPos.y, 'f', 2) + " " + QString::number(param.camPos.z, 'f', 2));
        renderString(strCamPos.toAscii(), 10, 20, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);

        QString strLightPos = ("LightPos: " + QString::number(param.lightPos.x, 'f', 2) + " " + QString::number(param.lightPos.y, 'f', 2) + " " + QString::number(param.lightPos.z, 'f', 2));
        renderString(strLightPos.toAscii(), 10, 40, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);

        QString strLightDir = ("LightDir: " + QString::number(param.lightDir.x, 'f', 2) + " " + QString::number(param.lightDir.y, 'f', 2) + " " + QString::number(param.lightDir.z, 'f', 2));
        renderString(strLightDir.toAscii(), 10, 60, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);

        QString strFrameSet= ("Current Frameset: " + m_cameraManager->currentFramesetName());
        renderString(strFrameSet.toAscii(), 200, 20, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);
       

        //bottom
		for(uint i=1; i<m_shortcuts.size()+1; ++i)
		{
			QString text = m_shortcuts[i-1];

			renderString(text.toAscii(), posX, posY, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);

			posY += stepY;
	        
			if((i%nrY) == 0)
			{
				posY = startY; 
				posX += stepX;
			}
		}

		QString fpsStr = QString("FPS: ") + QString::number(m_fps);
		renderString(fpsStr.toAscii(), m_width - 65, 15, vec4(1.0f, 1.0f, 1.0f, 1.0f), GLUT_BITMAP_HELVETICA_12);
	}

    glPopClientAttrib();
    glPopAttrib();

	m_fpsCount++;

    m_oldTime = frameTime;
}

void GUI::onTimer()
{
	m_fps = m_fpsCount * 5;
	m_fpsCount = 0;
}

void GUI::toggleMode()
{
	m_mode++;
	if(m_mode > 1)
		m_mode = 0;
}

int GUI::currentMode()
{
    return m_mode;
}

void GUI::setFontColor(const vec4 &color)
{
	m_fontColor = color;
}

void GUI::resize(int width, int height)
{
	m_width = width;
	m_height = height;
}

bool GUI::onMouseClick(uint mx, uint my)
{
    bool clicked = false;

    if(m_mode == 1)
    {    
	    for(uint i=0; i<m_guiElements.size(); ++i)
	    {
		    clicked |= m_guiElements[i]->onMouseClick(mx, my);
	    }
    }

	return clicked;
}

void GUI::onMouseMove(uint mx, uint my)
{
    if(m_mode == 1)
    {   
	    for(uint i=0; i<m_guiElements.size(); ++i)
	    {
		    m_guiElements[i]->onMouseMove(mx, my);
	    }
    }
}

void GUI::onMouseRelease()
{
    if(m_mode == 1)
    {   
	    for(uint i=0; i<m_guiElements.size(); ++i)
	    {
    		m_guiElements[i]->onMouseRelease();
	    }
    }
}