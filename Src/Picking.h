//Author: Sören Pirk
//Date: 22.01.2013

#ifndef PICKING
#define PICKING

#include "Misc.h"

class Camera;

class Picking
{
private:
    vec3 pickingRay(Camera* camera, int mouse_x, int mouse_y, float window_width, float window_height);
    void getPickingRay(const Transform &trans, const float fov, const float ncp, const float window_width, const float window_height, float mouse_x, float mouse_y, vec3 &rayPos, vec3 &rayDir);
    vec3 intersectionPlane(vec3 v1, vec3 v2, vec3 v3, vec3 startPos, vec3 endPos);
    bool intersectTriangle(vec3 rayStart, vec3 rayDir, vec3 a, vec3 b, vec3 c, float &t);
    bool intersectQuad(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, vec3 v3, float &t);
    bool intersectCube(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, vec3 v3, vec3 v4, vec3 v5, vec3 v6, vec3 v7, float &t);    

public:
    Picking();
    ~Picking();

    float select(const Transform &trans, const GlobalObjectParam &param, const mat4 &matModel, vec3 &mi, vec3 &ma, int width, int height, int mx, int my);
};

#endif