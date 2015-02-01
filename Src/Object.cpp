#include "Object.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "ModelLoaderObj.h"

Object::Object(const QString &fileName, const vec3 &pos, const vec3 &scale, const vec4 &rot, const vec4 &color)
: m_fileName(fileName),
  m_position(pos),
  m_scale(scale),
  m_rotation(rot),
  m_color(color),
  m_shaderTriangles(NULL),
  m_shaderLines(NULL),
  m_shaderTrianglesDepth(NULL),
  m_isSelected(false),
  m_max(math_minfloat, math_minfloat, math_minfloat),
  m_min(math_maxfloat, math_maxfloat, math_maxfloat),
  m_nrTriangles(0),
  m_nrVertices(0),
  m_up(0, 1, 0, 1)
{
    init();
}

Object::~Object()
{
    delete m_shaderTriangles;
    delete m_shaderLines;
    delete m_shaderTrianglesDepth;

    for(int i=m_vbosTriangles.size()-1; i>=0; --i)
    {
        VertexBufferObjectAttribs *vbo = m_vbosTriangles[i];
        delete vbo;
    }

    for(int i=m_vbosLines.size()-1; i>=0; --i)
    {
        VertexBufferObjectAttribs *vbo = m_vbosLines[i];
        delete vbo;
    }
}

void Object::init()
{
    buildVBOs(m_fileName, vec3(0, 0, 0), vec3(1, 1, 1));

    m_shaderTriangles = new Shader("Shader/Object.vert.glsl", "Shader/Object.frag.glsl");
    m_shaderTriangles ->bindAttribLocations();

    m_shaderTrianglesDepth = new Shader("Shader/ObjectDepth.vert.glsl", "Shader/ObjectDepth.frag.glsl");
    m_shaderTrianglesDepth ->bindAttribLocations();

    m_shaderLines = new Shader("Shader/ObjectLines.vert.glsl", "Shader/ObjectLines.frag.glsl");
    m_shaderLines->bindAttribLocations();
}

void Object::render(const Transform &trans, const GlobalObjectParam &param)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    mat4 model      = mat4::translate(m_position) * mat4::rotateY(m_rotation.y) * mat4::scale(m_scale); 
	mat4 view       = trans.view;
	mat4 projection = trans.projection;
	mat4 lightView  = trans.lightView;

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

    glEnable(GL_CLIP_DISTANCE0);    

	m_shaderTriangles->bind();  

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, param.shadowMapID);    
        m_shaderTriangles->seti("shadowMap", 1);   

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, param.shadowMapBlurredID);    
        m_shaderTriangles->seti("shadowMapBlurred", 2);  

	    m_shaderTriangles->setMatrix("matModel", model, GL_TRUE); 
	    m_shaderTriangles->setMatrix("matView", view, GL_TRUE);
	    m_shaderTriangles->setMatrix("matProjection", projection, GL_TRUE);
        m_shaderTriangles->setMatrix("matLightView", lightView, GL_TRUE);

        m_shaderTriangles->set3f("lightPos", param.lightPos);        
        m_shaderTriangles->set3f("camPos", param.camPos);      
        m_shaderTriangles->seti("applyShadow", param.applyShadow);
        m_shaderTriangles->setf("shadowIntensity", param.shadowIntensity);
        m_shaderTriangles->seti("isSelected", m_isSelected);

        m_shaderTrianglesDepth->set4f("clipPlane", param.clipPlaneGround);

        for(uint i=0; i<m_vbosTriangles.size(); ++i)
        {
            m_vbosTriangles[i]->render();
        }

	m_shaderTriangles->release();


    if(param.renderMesh)
    {
	    m_shaderLines->bind();  

	        m_shaderLines->setMatrix("matModel", model, GL_TRUE); 
	        m_shaderLines->setMatrix("matView", view, GL_TRUE);
	        m_shaderLines->setMatrix("matProjection", projection, GL_TRUE);

            for(uint i=0; i<m_vbosTriangles.size(); ++i)
            {
                m_vbosLines[i]->render();
            }

	    m_shaderLines->release();
    }

	glDisable(GL_CULL_FACE);    

	glPopClientAttrib();
	glPopAttrib();
}

void Object::renderDepth(const Transform &trans, const GlobalObjectParam &param)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    glEnable(GL_CLIP_DISTANCE0);

    mat4 model      = mat4::translate(m_position) * mat4::rotateY(m_rotation.y) * mat4::scale(m_scale);  
	mat4 view       = trans.view;
	mat4 projection = trans.projection;

    glDisable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);
    //glFrontFace(GL_CCW);

    //glClearDepth(1.0);
    //glEnable(GL_POLYGON_OFFSET_FILL);
    //glDepthFunc(GL_LEQUAL);
    //glDepthRange(param.depthRangeMin, param.depthRangeMax);
    //glPolygonOffset(param.polygonOffsetFactor, param.polygonOffsetUnits);
    
	m_shaderTrianglesDepth->bind();  

	    m_shaderTrianglesDepth->setMatrix("matModel", model, GL_TRUE); 
	    m_shaderTrianglesDepth->setMatrix("matView", view, GL_TRUE);
	    m_shaderTrianglesDepth->setMatrix("matProjection", projection, GL_TRUE);     

        m_shaderTrianglesDepth->set4f("clipPlane", param.clipPlaneGround);

        for(uint i=0; i<m_vbosTriangles.size(); ++i)
        {
            m_vbosTriangles[i]->render();
        }

	m_shaderTrianglesDepth->release();

	glPopClientAttrib();
	glPopAttrib();
}


void Object::buildVBOs(const QString &fileName, const vec3 &rot, const vec3 &scale)
{
    ModelOBJ *model = new ModelOBJ();

    qDebug() << "OBSTACLEOBJ::load():" << fileName.toAscii();
    model->import(fileName.toAscii());
    
	uint nrMeshes = model->getNumberOfMeshes();

    const ModelOBJ::Vertex *vb = model->getVertexBuffer();
    const int *tempIdx = model->getIndexBuffer();

    mat4 rotMatX = mat4::rotate(rot.x, 1.0f, 0.0f, 0.0f);
	mat4 rotMatY = mat4::rotate(rot.y, 0.0f, 1.0f, 0.0f);
    mat4 rotMatZ = mat4::rotate(rot.z, 0.0f, 0.0f, 1.0f);
    mat4 scaleMat = mat4::scale(scale.x, scale.y, scale.z);

	for(uint i = 0; i < nrMeshes; ++i)	
	{
		vector<vec3> tmpVertices;	
		vector<vec3> tmpNormals;
		vector<vec3> tmpTexCoords;

		const ModelOBJ::Mesh &objMesh = model->getMesh(i);
		int startIndex = objMesh.startIndex;
		m_nrTriangles = objMesh.triangleCount;
		m_nrVertices = objMesh.triangleCount * 3;

		for(int j=startIndex; j<startIndex + m_nrVertices; ++j)
		{
			uint idx = tempIdx[j];

			Vector3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
			Vector3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
			Vector3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);            

			tmpVertices.push_back(v);
			tmpNormals.push_back(n);
			tmpTexCoords.push_back(t);
		} 

        vec3 mi(math_maxfloat, math_maxfloat, math_maxfloat);
        vec3 ma(math_minfloat, math_minfloat, math_minfloat);

        for(uint i=0; i<tmpVertices.size(); ++i)
        {
            vec3 v = tmpVertices[i];
            
            if(v.x > m_max.x)
                m_max.x = v.x;

            if(v.y > m_max.y)
                m_max.y = v.y;

            if(v.z > m_max.z)
                m_max.z = v.z;

            if(v.x < m_min.x)
                m_min.x = v.x;

            if(v.y < m_min.y)
                m_min.y = v.y;

            if(v.z < m_min.z)
                m_min.z = v.z;
        }        

        m_center =  (vec4(m_min) + vec4(m_max)) * 0.5;

        VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tmpVertices.size()];

		for(uint i=0; i<tmpVertices.size(); ++i)
		{
            vec3 v = tmpVertices[i];
            vec3 n = tmpNormals[i];
            vec3 t = tmpTexCoords[i];

			data[i].vx = v.x;
			data[i].vy = v.y;
			data[i].vz = v.z;
			data[i].vw = 1.0f;

			data[i].cx = m_color.x;
			data[i].cy = m_color.y;
			data[i].cz = m_color.z;
			data[i].cw = m_color.w;
			
            data[i].nx = n.x;
			data[i].ny = n.y;
			data[i].nz = n.z;
			data[i].nw = 1.0f;
            
			data[i].tx = t.x;
			data[i].ty = t.y;
            data[i].tz = 0.0f;
            data[i].tw = 0.0f;
		}

		VertexBufferObjectAttribs* vboMesh = new VertexBufferObjectAttribs();
		vboMesh->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_TRIANGLES); 

		vboMesh->addAttrib(VERTEX_POSITION);
		vboMesh->addAttrib(VERTEX_NORMAL);
		vboMesh->addAttrib(VERTEX_COLOR);
		vboMesh->addAttrib(VERTEX_TEXTURE);
		vboMesh->bindAttribs();


		VertexBufferObjectAttribs* vboLines = new VertexBufferObjectAttribs();
		vboLines->setData(data, GL_STATIC_DRAW, tmpVertices.size(), GL_LINES); 

		vboLines->addAttrib(VERTEX_POSITION);
		vboLines->addAttrib(VERTEX_NORMAL);
		vboLines->addAttrib(VERTEX_COLOR);
		vboLines->addAttrib(VERTEX_TEXTURE);
		vboLines->bindAttribs();

		delete[] data;

        m_vbosTriangles.push_back(vboMesh);
        m_vbosLines.push_back(vboLines);
    }
}
