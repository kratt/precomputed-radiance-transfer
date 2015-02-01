//Author: Sören Pirk
//Date: 22.01.2013

#include "Renderer.h"
#include "GUI.h"
#include "CameraManager.h"
#include "FrameBufferObjectMultisample.h"
#include "FrameBufferObject.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Scene.h"
#include "Mesh.h"
#include "Light.h"
#include "Texture.h"

Renderer::Renderer(Scene *scene, CameraManager *camManager, GUI *gui)
: m_scene(scene),
  m_gui(gui),
  m_cameraManager(camManager),
  m_bgColor(0.1f, 0.1f, 0.1f, 1.0f),
  m_width(0),
  m_height(0),
  m_samples(16),
  m_bgMode(0)
{
    init();
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
}

void Renderer::render(Transform &trans, GlobalObjectParam &param)
{
    if(param.applyShadow)
    {
        m_scene->m_light->renderLightView(param, trans.lightView, param.shadowMapID, param.shadowMapBlurredID); 
    }    

    renderScene(trans, param);

	m_gui->render(param);

    //renderTexture(m_scene->m_light->m_fboLight->texAttachment(GL_COLOR_ATTACHMENT0), 400, 100, 400, 300);
}

void Renderer::renderScene(const Transform &trans, const GlobalObjectParam &param)
{
    glViewport(0, 0, m_width, m_height);
    glClearColor(m_bgColor.x, m_bgColor.y, m_bgColor.z, m_bgColor.w);    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     

    glEnable(GL_MULTISAMPLE);        

    if(param.polygonMode == 1)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
    m_scene->renderObjects(trans, param);
    m_scene->renderWorld(trans, param);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;    
}

void Renderer::toggleBGColor()
{
    m_bgMode ++;
    if(m_bgMode > 2)
        m_bgMode = 0;

    if(m_bgMode == 0)
	{
        m_bgColor = vec4(0.1f, 0.1f, 0.1f, 1.0f);
		m_gui->setFontColor(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	}
   
    if(m_bgMode == 1)
	{
        m_bgColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_gui->setFontColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}

    if(m_bgMode == 2)
	{
        m_bgColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_gui->setFontColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

