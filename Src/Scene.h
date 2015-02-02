//Author: Sören Pirk
//Date: 22.01.2013

#ifndef SCENE
#define SCENE

#include "Misc.h"

class NiceGrid;
class Light;
class VertexBufferObjectAttribs;
class Shader;
class CameraManager;
class Object;

class Scene
{
public:
    Scene(CameraManager *camManager);
    ~Scene();

	void update(float delta);
    void init();
    void initShaders();

    void renderWorld(const Transform &trans, const GlobalObjectParam &param);  
    void renderObjects(const Transform &trans, const GlobalObjectParam &param);  
    void renderObjectsDepth(const Transform &trans, const GlobalObjectParam &param);

	void renderMesh(const Transform &trans, const GlobalObjectParam &param);
	void renderSkyDome(const Transform &trans, const GlobalObjectParam &param);
	void renderDebug(const Transform &trans, const GlobalObjectParam &param);

	// PRT stuff
	void initLightProbe(const char* fileName);

	void generateSamples();
	void precomputeSHFunctions();
	void projectLightFunction();
	void projectUnshadowed();
	void projectShadowed();

	void lightProbeAccess(vec3 &color, vec3 direction);
	bool visibility(int vertIdx, const vec3 &dir);
	
public:
    Light *m_light;

private:
	void buildVBOMesh();

    CameraManager *m_cameraManager;
	VertexBufferObjectAttribs *m_vboMesh;
	VertexBufferObjectAttribs *m_vboSkyDome;

	Shader *m_shaderMesh;
	Shader *m_shaderSkyDome;
    Shader *m_shaderNormal;
    Shader *m_shaderDepth;

	float *m_lightProbe;
	float m_lightProbeWidth;
	float m_lightProbeHeight;

	GLuint m_texLightProbe;

	std::map<uint, MeshFace> m_faces;
	std::map<uint, MeshVertex> m_vertices;

	Sampler *m_sampler;
	std::vector<vec3> m_lightCoeffs;
	std::vector< std::vector<vec3> > m_transCoeffs;
	int m_numSamples;
	int m_numBands;

	std::vector<vec3> m_debugSampleDirs;
	std::vector<vec3> m_debugSampleColor;
};

#endif

 