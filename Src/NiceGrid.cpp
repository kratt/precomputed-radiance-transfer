//Author: Sören Pirk
//Date: 22.01.2013

#include "NiceGrid.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Texture.h"
#include "Mesh.h"

NiceGrid::NiceGrid(GLfloat size, GLfloat rep)
: m_size(size),
  m_rep(rep),
  m_shader(NULL),
  m_texture(NULL),
  m_backFaceAlpha(0.2),
  m_ambient(0.12f, 0.12f, 0.1f),
  m_diffuse(1.0f, 1.0f, 0.9f),
  m_vbo(NULL),
  m_position(0.0f, 0.0f, 0.0f)
{
    QImage tex("Data/floor_blue.png");

    m_texture = new Texture(tex);
    m_texture->setWrapMode(GL_REPEAT);

    m_shader = new Shader("Shader/NiceGrid.vert.glsl", "Shader/NiceGrid.frag.glsl");
    m_shader->bindAttribLocations();

    createVBO();
}

NiceGrid::~NiceGrid()
{
    delete m_shader;
    delete m_texture;
}

void NiceGrid::render(const Transform &trans, const GlobalObjectParam &param)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    mat4 model = mat4::translate(m_position);
    mat4 view = trans.view;
    mat4 projection = trans.projection;
    mat4 viewProjection = trans.viewProjection;
    mat4 lightView = trans.lightView;

    glEnable(GL_TEXTURE_2D);

    m_shader->bind();  

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture->id());    
        m_shader->seti("tex", 0);    

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, param.shadowMapID);    
        m_shader->seti("shadowMap", 1);   

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, param.shadowMapBlurredID);    
        m_shader->seti("shadowMapBlurred", 2);   

        m_shader->set3f("lightPos", param.lightPos);
        m_shader->set3f("lightDir", param.lightDir);
        m_shader->set3f("camPos", param.camPos);
        m_shader->set3f("diffuseColor", m_diffuse.x, m_diffuse.y, m_diffuse.z);
        m_shader->set3f("ambientColor", m_ambient.x, m_ambient.z, m_ambient.z);
        m_shader->setf("alpha", 1.0);
        m_shader->seti("applyShadow", param.applyShadow);
        m_shader->seti("renderMode", param.gridRenderMode);
        m_shader->setf("shadowIntensity", param.shadowIntensity);

        m_shader->setMatrix("matModel", model, GL_TRUE); 
        m_shader->setMatrix("matView", view, GL_TRUE);
        m_shader->setMatrix("matProjection", projection, GL_TRUE);
        m_shader->setMatrix("matLightView", lightView, GL_TRUE);
        m_shader->setMatrix("matViewProjection", viewProjection, GL_TRUE);

        glEnable(GL_CULL_FACE);    
        glCullFace(GL_BACK);

        m_vbo->render();

        glCullFace(GL_FRONT);
        m_shader->setf("alpha", m_backFaceAlpha);
        m_shader->seti("renderMode", 1);

        m_vbo->render();

    m_shader->release();

    glPopClientAttrib();
    glPopAttrib();
}

void NiceGrid::setAmbientColor(float r, float g, float b)
{
    m_ambient.x = r;
    m_ambient.y = g;
    m_ambient.z = b;
}

void NiceGrid::setDiffuseColor(float r, float g, float b)
{
    m_diffuse.x = r;
    m_diffuse.y = g;
    m_diffuse.z = b;
}

void NiceGrid::createVBO()
{
    m_shader->bindAttribLocation("Position", VERTEX_POSITION);
    m_shader->bindAttribLocation("Normal", VERTEX_NORMAL);
    m_shader->bindAttribLocation("Color", VERTEX_COLOR);
    m_shader->bindAttribLocation("Texture", VERTEX_TEXTURE);

    uint nrVertices = 4;
    VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[nrVertices];

    data[0].vx = -m_size;
    data[0].vy = 0.0f;
    data[0].vz = -m_size;
    data[0].vw = 1.0f;

    data[0].nx = 0.0f;
    data[0].ny = 1.0f;
    data[0].nz = 0.0f;
    data[0].nw = 0.0f;

    data[0].cx = 1.0f;
    data[0].cy = 1.0f;
    data[0].cz = 1.0f;
    data[0].cw = 1.0f;

    data[0].tx = 0.0f;
    data[0].ty = 0.0f;
    data[0].tz = 0.0f;
    data[0].tw = 0.0f;


    data[1].vx = -m_size;
    data[1].vy = 0.0f;
    data[1].vz = m_size;
    data[1].vw = 1.0f;

    data[1].nx = 0.0f;
    data[1].ny = 1.0f;
    data[1].nz = 0.0f;
    data[1].nw = 0.0f;

    data[1].cx = 1.0f;
    data[1].cy = 1.0f;
    data[1].cz = 1.0f;
    data[1].cw = 1.0f;

    data[1].tx = 0.0f;
    data[1].ty = m_rep;
    data[1].tz = 0.0f;
    data[1].tw = 0.0f;


    data[2].vx = m_size;
    data[2].vy = 0.0f;
    data[2].vz = m_size;
    data[2].vw = 1.0f;

    data[2].nx = 0.0f;
    data[2].ny = 1.0f;
    data[2].nz = 0.0f;
    data[2].nw = 0.0f;

    data[2].cx = 1.0f;
    data[2].cy = 1.0f;
    data[2].cz = 1.0f;
    data[2].cw = 1.0f;

    data[2].tx = m_rep;
    data[2].ty = m_rep;
    data[2].tz = 0.0f;
    data[2].tw = 0.0f;


    data[3].vx = m_size;
    data[3].vy = 0.0f;
    data[3].vz = -m_size;
    data[3].vw = 1.0f;

    data[3].nx = 0.0f;
    data[3].ny = 1.0f;
    data[3].nz = 0.0f;
    data[3].nw = 0.0f;

    data[3].cx = 1.0f;
    data[3].cy = 1.0f;
    data[3].cz = 1.0f;
    data[3].cw = 1.0f;

    data[3].tx = m_rep;
    data[3].ty = 0.0f;
    data[3].tz = 0.0f;
    data[3].tw = 0.0f;

    m_vbo = new VertexBufferObjectAttribs();
    m_vbo->setData(data, GL_STATIC_DRAW, nrVertices, GL_QUADS);

    m_vbo->addAttrib(VERTEX_POSITION);
    m_vbo->addAttrib(VERTEX_NORMAL);
    m_vbo->addAttrib(VERTEX_COLOR);
    m_vbo->addAttrib(VERTEX_TEXTURE);
    m_vbo->bindAttribs();

    delete[] data;
}