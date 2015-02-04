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

struct MeshFaceData
{
	float vax, vay, vaz, vaw;
	float vbx, vby, vbz, vbw;
	float vcx, vcy, vcz, vcw;
};

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
	bool visibility(const vec3 &rayOrigin, const vec3 &dir, float &dist);
	bool visibilityGPU(const vec3 &p, const vec3 &dir, float& dist);

	void initSSBOs();
	
public:
    Light *m_light;

private:
	void buildVBOMesh();
	void ambientOcclusion();

	void debugIsOccluded();
	bool intersectTriangle(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, float &t);

    CameraManager *m_cameraManager;
	VertexBufferObjectAttribs *m_vboMesh;
	VertexBufferObjectAttribs *m_vboSkyDome;

	Shader *m_shaderMesh;
	Shader *m_shaderSkyDome;
    Shader *m_shaderNormal;
    Shader *m_shaderDepth;
	Shader *m_shaderRayIntersection;


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

	vec3 m_debugRayOrigin;

	std::vector<vec3> m_debugSampleDirs;
	std::vector<vec3> m_debugSampleColor;
	std::vector<vec3> m_debugIntersectionPoints;
	std::vector<float> m_debugDistToIntersections;

	GLuint m_ssboFaceData;
	GLuint m_ssboIntersectionResult;
	GLuint m_atomicBufferHitCounter;
	int m_numLocalWorkGroups;
};

#endif

 