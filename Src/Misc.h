//Author: Sören Pirk
//Date: 22.01.2013

#ifndef MISC
#define MISC

#include <GL/glew.h>
#include <QGLWidget>
#include <QtCore>
#include <QtGui>
#include <GL/glut.h>

#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <limits>
#include <iostream>

#include "Global.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "HPTimer.h"

using namespace std;

class VertexBufferObjectAttribs;

#undef min
#undef max

struct MeshFace
{
	uint id;

	uint a;
	uint b;
	uint c;

	vec3 center;
	vec3 n;
};

struct MeshVertex
{
	vec3 pos;
	vec3 normal;
	vec3 texCoords;
	vec3 prt;

	uint id;
	std::vector<uint> faceIds;
	std::vector<uint> vertIds;
};

struct Spherical
{
	float theta;
	float phi;
};

struct Sample
{
	Spherical spherical_coord;
	vec3 cartesian_coord;
	float* sh_functions; 
};

struct Sampler
{
	Sample* samples;
	int number_of_samples;
};

struct Transform
{
    Matrix4x4 view;
    Matrix4x4 projection;
    Matrix4x4 lightView;
    Matrix3x3 normal;
    Matrix4x4 viewProjection;
};

struct GlobalObjectParam
{
    vec3 lightPos;
    vec3 lightDir;
    vec3 camPos;
    float shadowIntensity;
    bool applyShadow;
    int gridRenderMode;
    int polygonMode;
    bool renderMesh;
    bool renderObjects;
    float ncp;
    float fcp;
    float fov;

    float depthRangeMax;
    float depthRangeMin;
    float polygonOffsetFactor;
    float polygonOffsetUnits;

    vec4 clipPlaneGround;

    GLuint shadowMapID;
    GLuint shadowMapBlurredID;

    int windowWidth;
    int windowHeight;
};

void glEnable2D(void);
void glDisable2D(void);
void glEnableFixedFunction(const Transform &trans);
void glDisableFixedFunction();

float frand(float low, float high);
int   irand(int low, int high);

float lerp(float a, float b, float s);
vec3  lerp(const vec3 &a, const vec3 &b, float s);
float cosineInterpolation(float a, double b, double s);
double hermiteInterpolation(double y0, double y1, double y2,double y3, double mu, double tension, double bias);

void renderTexture(uint texture, uint posX, uint posY, float width, float height);
void renderQuad(float size, float r, float g, float b, float a);
void renderQuad(float width, float height, float r, float g, float b, float a);
void renderQuad(float posX, float posY, float width, float height);
void renderOrigin(float lineWidth);
void screenSizeQuad(float width, float height, float fov);

void renderString(const char *str, int x, int y, vec4 &color, void *font = GLUT_BITMAP_HELVETICA_18);
void renderString(const char *str, int x, int y, float r, float g, float b, float a, void *font = GLUT_BITMAP_HELVETICA_18);

void smoothBackground(vec4 top, vec4 bottom, float windowWidth, float windowHeight);

void saveFrameBuffer(QGLWidget *widget);
void saveFrameBuffer(QGLWidget *widget, int idx);

void getCameraFrame(const Transform &trans, vec3 &dir, vec3 &up, vec3 &right, vec3 &pos);
vec3 getCamPosFromModelView(const Transform &trans);
vec3 getViewDirFromModelView(const Transform &trans);
vec3 getUpDirFromModelView(const Transform &trans);

void checkGLError();
void checkGLVersion();

static float colorJet[] = {0.000000f, 0.000000f, 0.562500f, 0.000000f, 0.000000f, 0.625000f, 0.000000f, 0.000000f, 0.687500f, 0.000000f, 0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.812500f, 0.000000f, 0.000000f, 0.875000f, 0.000000f, 0.000000f, 0.937500f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.062500f, 1.000000f, 0.000000f, 0.125000f, 1.000000f, 0.000000f, 0.187500f, 1.000000f, 0.000000f, 0.250000f, 1.000000f, 0.000000f, 0.312500f, 1.000000f, 0.000000f, 0.375000f, 1.000000f, 0.000000f, 0.437500f, 1.000000f, 0.000000f, 0.500000f, 1.000000f, 0.000000f, 0.562500f, 1.000000f, 0.000000f, 0.625000f, 1.000000f, 0.000000f, 0.687500f, 1.000000f, 0.000000f, 0.750000f, 1.000000f, 0.000000f, 0.812500f, 1.000000f, 0.000000f, 0.875000f, 1.000000f, 0.000000f, 0.937500f, 1.000000f, 0.000000f, 1.000000f, 1.000000f, 0.062500f, 1.000000f, 0.937500f, 0.125000f, 1.000000f, 0.875000f, 0.187500f, 1.000000f, 0.812500f, 0.250000f, 1.000000f, 0.750000f, 0.312500f, 1.000000f, 0.687500f, 0.375000f, 1.000000f, 0.625000f, 0.437500f, 1.000000f, 0.562500f, 0.500000f, 1.000000f, 0.500000f, 0.562500f, 1.000000f, 0.437500f, 0.625000f, 1.000000f, 0.375000f, 0.687500f, 1.000000f, 0.312500f, 0.750000f, 1.000000f, 0.250000f, 0.812500f, 1.000000f, 0.187500f, 0.875000f, 1.000000f, 0.125000f, 0.937500f, 1.000000f, 0.062500f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.937500f, 0.000000f, 1.000000f, 0.875000f, 0.000000f, 1.000000f, 0.812500f, 0.000000f, 1.000000f, 0.750000f, 0.000000f, 1.000000f, 0.687500f, 0.000000f, 1.000000f, 0.625000f, 0.000000f, 1.000000f, 0.562500f, 0.000000f, 1.000000f, 0.500000f, 0.000000f, 1.000000f, 0.437500f, 0.000000f, 1.000000f, 0.375000f, 0.000000f, 1.000000f, 0.312500f, 0.000000f, 1.000000f, 0.250000f, 0.000000f, 1.000000f, 0.187500f, 0.000000f, 1.000000f, 0.125000f, 0.000000f, 1.000000f, 0.062500f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.937500f, 0.000000f, 0.000000f, 0.875000f, 0.000000f, 0.000000f, 0.812500f, 0.000000f, 0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.687500f, 0.000000f, 0.000000f, 0.625000f, 0.000000f, 0.000000f, 0.562500f, 0.000000f, 0.000000f, 0.500000f, 0.000000f, 0.000000f};
static float colorHot[] = {0.041667f, 0.000000f, 0.000000f, 0.083333f, 0.000000f, 0.000000f, 0.125000f, 0.000000f, 0.000000f, 0.166667f, 0.000000f, 0.000000f, 0.208333f, 0.000000f, 0.000000f, 0.250000f, 0.000000f, 0.000000f, 0.291667f, 0.000000f, 0.000000f, 0.333333f, 0.000000f, 0.000000f, 0.375000f, 0.000000f, 0.000000f, 0.416667f, 0.000000f, 0.000000f, 0.458333f, 0.000000f, 0.000000f, 0.500000f, 0.000000f, 0.000000f, 0.541667f, 0.000000f, 0.000000f, 0.583333f, 0.000000f, 0.000000f, 0.625000f, 0.000000f, 0.000000f, 0.666667f, 0.000000f, 0.000000f, 0.708333f, 0.000000f, 0.000000f, 0.750000f, 0.000000f, 0.000000f, 0.791667f, 0.000000f, 0.000000f, 0.833333f, 0.000000f, 0.000000f, 0.875000f, 0.000000f, 0.000000f, 0.916667f, 0.000000f, 0.000000f, 0.958333f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.041667f, 0.000000f, 1.000000f, 0.083333f, 0.000000f, 1.000000f, 0.125000f, 0.000000f, 1.000000f, 0.166667f, 0.000000f, 1.000000f, 0.208333f, 0.000000f, 1.000000f, 0.250000f, 0.000000f, 1.000000f, 0.291667f, 0.000000f, 1.000000f, 0.333333f, 0.000000f, 1.000000f, 0.375000f, 0.000000f, 1.000000f, 0.416667f, 0.000000f, 1.000000f, 0.458333f, 0.000000f, 1.000000f, 0.500000f, 0.000000f, 1.000000f, 0.541667f, 0.000000f, 1.000000f, 0.583333f, 0.000000f, 1.000000f, 0.625000f, 0.000000f, 1.000000f, 0.666667f, 0.000000f, 1.000000f, 0.708333f, 0.000000f, 1.000000f, 0.750000f, 0.000000f, 1.000000f, 0.791667f, 0.000000f, 1.000000f, 0.833333f, 0.000000f, 1.000000f, 0.875000f, 0.000000f, 1.000000f, 0.916667f, 0.000000f, 1.000000f, 0.958333f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 1.000000f, 0.062500f, 1.000000f, 1.000000f, 0.125000f, 1.000000f, 1.000000f, 0.187500f, 1.000000f, 1.000000f, 0.250000f, 1.000000f, 1.000000f, 0.312500f, 1.000000f, 1.000000f, 0.375000f, 1.000000f, 1.000000f, 0.437500f, 1.000000f, 1.000000f, 0.500000f, 1.000000f, 1.000000f, 0.562500f, 1.000000f, 1.000000f, 0.625000f, 1.000000f, 1.000000f, 0.687500f, 1.000000f, 1.000000f, 0.750000f, 1.000000f, 1.000000f, 0.812500f, 1.000000f, 1.000000f, 0.875000f, 1.000000f, 1.000000f, 0.937500f, 1.000000f, 1.000000f, 1.000000f};

void colorMap(float x, float * out, float * cm);
void colorMapBgr(float x, float * out, float * cm);

vector<VertexBufferObjectAttribs*> loadObj(QString fileName);
void loadObjData(const char *fileName, std::map<uint, MeshFace> &faces, std::map<uint, MeshVertex> &vertices, bool normalizeData);
void normalize(std::map<uint, MeshVertex> &vertices);

float Legendre(int l, int m, float x);
float DoubleFactorial(int n);
float SphericalHarmonic(int l, int m, float theta, float phi);
float K(int l, int m);
int factorial(int n);

bool rayIntersectsTriangle(const vec3 &p, const vec3 &d, const vec3 &v0, const vec3 &v1, const vec3 &v2);


#endif

