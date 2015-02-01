#ifndef OBJECT
#define OBJECT

#include "Misc.h"

class Shader;
class VertexBufferObjectAttribs;

class Object
{
public:
    Object(const QString &fileName, const vec3 &pos = vec3(), const vec3 &scale = vec3(1, 1, 1), const vec4 &rot = vec4(), const vec4 &color = vec4(1, 1, 1, 1));
    ~Object();

    void render(const Transform &trans, const GlobalObjectParam &param);
    void renderDepth(const Transform &trans, const GlobalObjectParam &param);

    bool m_isSelected;
    vec3 m_min;
    vec3 m_max;  
    vec3 m_position;
    vec4 m_rotation;
    vec3 m_scale;
    vec4 m_color;
    vec4 m_center;
    vec4 m_up;
    vec4 m_right;
    vec4 m_front;

private:
    void init();
    void buildVBOs(const QString &fileName, const vec3 &rot, const vec3 &scale);

private:
    QString m_fileName;
    Shader *m_shaderTriangles;
    Shader *m_shaderTrianglesDepth;
    Shader *m_shaderLines;   

    vector<VertexBufferObjectAttribs *> m_vbosTriangles;
    vector<VertexBufferObjectAttribs *> m_vbosLines;

    int m_nrTriangles;
    int m_nrVertices;
};

#endif