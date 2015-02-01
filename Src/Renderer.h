//Author: Sören Pirk
//Date: 22.01.2013

#ifndef RENDERER
#define RENDERER

#include "Misc.h"

class GUI;
class CameraManager;
class FrameBufferObjectMultisample;
class FrameBufferObject;
class Shader;
class Scene;
class VertexBufferObjectAttribs;
class Texture;

class Renderer
{
public:
    Renderer(Scene *scene, CameraManager *camManager, GUI *gui);
    ~Renderer();

    void init();
    void render(Transform &trans, GlobalObjectParam &param);
    
    void resize(int width, int height);
    void toggleBGColor();
    void togglePolygonMode();

private:
    void renderScene(const Transform &trans, const GlobalObjectParam &param);

private:
    GUI *m_gui;
    CameraManager *m_cameraManager;
	Scene *m_scene;

    int m_width;
    int m_height;

    vec4 m_bgColor; 

    const GLuint m_samples;
    GLuint m_bgMode;
};

#endif

 