//Author: Sören Pirk
//Date: 22.01.2013

#ifndef LIGHT
#define LIGHT

#include "Misc.h"
#include "Spline.h"

class CameraManager;
class VertexBufferObjectAttribs;
class Shader;
class FrameBufferObject;
class Scene;

class Light
{
public:
	struct Path
	{
		QString name;
		vector<vec3> positions;
	};

   Light(Scene *scene, const vec3 &pos);
   ~Light();
    
   void blurShadowMap();
   void renderLightView(const GlobalObjectParam &param, mat4 &lightView, GLuint &shadowMapID, GLuint &shadowMapBlurredID);

   void render(const Transform &trans, const GlobalObjectParam &param);
   vec3 position();

   void setPosition(const vec3 &pos);
   void move(CameraManager *camManager, float diffX, float diffY);

   void loadPaths();
   void savePaths();
   void recordPath(bool record);
   void autoMove();
   void toggleMode();
   bool hasMoved();

   void update(float delta);

   bool m_moved;

private:

	vec3 m_position;
	float m_renderSize;

	vector<Path> m_paths;
    Path m_curPath;

    bool m_record;
    bool m_first;
    bool m_saved;

    DWORD m_oldTime;

    int m_moveMode;

    float m_height;
    float m_angle;
    float m_radius;

    vec3 m_oldPosition;
    Spline m_spline;
    float m_time;

    float m_speed;
    float m_distance;
    float m_movement;

    VertexBufferObjectAttribs *m_vbo;
    VertexBufferObjectAttribs *m_vboBlur;

    Shader *m_shader;    
    Shader *m_shaderBlur;    

public:
    FrameBufferObject *m_fboLight;
    FrameBufferObject *m_fboBlurV;
    FrameBufferObject *m_fboBlurH;   

    int m_bufferWidth;
    int m_bufferHeight;
    float m_fcpLight;
    float m_ncpLight;
    float m_fovLight;

    Scene *m_scene;

    mat4 m_lightView;
};

#endif

