#include "Scene.h"
#include "NiceGrid.h"
#include "Light.h"
#include "Shader.h"
#include "VertexBufferObjectAttribs.h"
#include "Mesh.h"
#include "CameraManager.h"
#include "Object.h"
#include "Picking.h"
#include "HDRLoader.h"
#include "Mesh.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

Scene::Scene(CameraManager *camManager)
: m_cameraManager(camManager),
  m_vboMesh(NULL),
  m_sampler(NULL),
  m_numSamples(16),
  m_numBands(4),
  m_numLocalWorkGroups(1)
{
	m_vboSkyDome = Mesh::sphere(10.0, 10);

    init();   
	initLightProbe("Data/LightProbes/uffizi_probe.hdr");

	loadObjData("Data/Objs/head.obj", m_faces, m_vertices, true);
	
	generateSamples();
	initSSBOs();
	
	//int l = 4;
	//int m = 0;

	//float theta = 10.0f;
	//float phi = 0.0f;
 //
	//float tmp = 3.0/16.0 * sqrt(1.0/math_pi) * (35.0f * pow(cos(theta), 4) - 30.0f*cos(theta)*cos(theta) + 3);

	//qDebug() << "SphericalHarmonic: " << SphericalHarmonic(l, m, theta, phi) << tmp;

	//GLint size;
	////glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &size); // max. count of local working groups
	////glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &size); // max. count of local working groups
	//glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &size); // max. count of local invocation 


	precomputeSHFunctions();
	projectLightFunction();
	projectUnshadowed();
	//projectShadowed();

	//ambientOcclusion();
	buildVBOMesh();
//	debugIsOccluded();
}

Scene::~Scene()
{

}

void Scene::initShaders()
{
	m_shaderMesh = new Shader("Shader/Mesh40.vert.glsl", "Shader/Mesh40.frag.glsl");
    m_shaderMesh->bindAttribLocations();

	m_shaderSkyDome = new Shader("Shader/SkyDome40.vert.glsl", "Shader/SkyDome40.frag.glsl");
    m_shaderSkyDome->bindAttribLocations();

    m_shaderNormal = new Shader("Shader/DefaultLight.vert.glsl", "Shader/DefaultLight.frag.glsl");
    m_shaderNormal->bindAttribLocations();

    m_shaderDepth = new Shader("Shader/Default.vert.glsl", "Shader/DefaultDepth.frag.glsl");
    m_shaderDepth->bindAttribLocations();

	m_shaderRayIntersection = new Shader();
	m_shaderRayIntersection->attachComputeShader("Shader/RayIntersection44.comp.glsl");
}

void Scene::initLightProbe(const char* fileName)
{
	HDRLoaderResult result;
	bool ret = HDRLoader::load(fileName, result); 

	m_lightProbe = result.cols;
	m_lightProbeWidth = result.width;
	m_lightProbeHeight = result.height;

	qDebug() << "Scene::initLightProbe(): Width:" << m_lightProbeWidth << ", height:" << m_lightProbeHeight;
	// init tex
	float *data = new float[m_lightProbeWidth * m_lightProbeHeight* 4];

	for(int y=0; y < m_lightProbeHeight; ++y)
	{
		for(int x=0; x < m_lightProbeWidth; ++x)
		{
			// flip y coordinate
			uint idx = 4 * ((m_lightProbeHeight-1-y) * m_lightProbeWidth + x);
			uint tmpIdx = 3 * (y * m_lightProbeWidth + x);

			data[idx]   = m_lightProbe[tmpIdx];
			data[idx+1] = m_lightProbe[tmpIdx+1];
			data[idx+2] = m_lightProbe[tmpIdx+2];
			data[idx+3] = 1.0f;
		}
	}

	glGenTextures(1, &m_texLightProbe);	
    glBindTexture(GL_TEXTURE_2D, m_texLightProbe);    

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_lightProbeWidth, m_lightProbeHeight, 0, GL_RGBA, GL_FLOAT, data);     
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);   

    delete[] data;
}

void Scene::init()
{
    m_light    = new Light(this, vec3(0.0f, 20.0f, 0.1f));
    initShaders();
}

void Scene::initSSBOs()
{
	int numFaces = m_faces.size();

	glGenBuffers(1, &m_ssboFaceData);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboFaceData);
	glBufferData(GL_SHADER_STORAGE_BUFFER , numFaces * sizeof(MeshFaceData), NULL, GL_STATIC_DRAW);

	MeshFaceData *data = (MeshFaceData*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numFaces * sizeof(MeshFaceData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

	for(std::map<uint, MeshFace>::iterator iterFaces=m_faces.begin(); iterFaces!=m_faces.end(); ++iterFaces)
	{
		MeshVertex &va = m_vertices.find(iterFaces->second.a)->second;
		MeshVertex &vb = m_vertices.find(iterFaces->second.b)->second;
		MeshVertex &vc = m_vertices.find(iterFaces->second.c)->second;

		int curIdx = iterFaces->second.id;

		data[curIdx].vax = va.pos.x;
		data[curIdx].vay = va.pos.y;
		data[curIdx].vaz = va.pos.z;
		data[curIdx].vaw = va.id;

		data[curIdx].vbx = vb.pos.x;
		data[curIdx].vby = vb.pos.y;
		data[curIdx].vbz = vb.pos.z;
		data[curIdx].vbw = vb.id;

		data[curIdx].vcx = vc.pos.x;
		data[curIdx].vcy = vc.pos.y;
		data[curIdx].vcz = vc.pos.z;
		data[curIdx].vcw = vc.id;

		data[curIdx].idx = curIdx;
		data[curIdx].idy = 0.0f;
		data[curIdx].idz = 0.0f;
		data[curIdx].idw = 0.0f;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);


	// init sample dirs ssbo
	std::vector<vec3> sampleDirs;
	for(int i=0; i<m_sampler->number_of_samples; ++i)
	{
		Sample& sample = m_sampler->samples[i];
		sampleDirs.push_back(sample.cartesian_coord);
	}
	//std::random_shuffle ( sampleDirs.begin(), sampleDirs.end());

	int numSampleDirs = sampleDirs.size();

	glGenBuffers(1, &m_ssboSampleDirs);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboSampleDirs);
	glBufferData(GL_SHADER_STORAGE_BUFFER , 4 * numSampleDirs * sizeof(float), NULL, GL_STATIC_DRAW);

	float *sampleDirData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 4 * numSampleDirs * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	
	for(int i=0; i<numSampleDirs; ++i)
	{
		vec3 dir = sampleDirs[i];
		int idx = 4*i;

		sampleDirData[idx]   = dir.x;
		sampleDirData[idx+1] = dir.y;
		sampleDirData[idx+2] = dir.z;
		sampleDirData[idx+3] = i;
	}
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);

	// init counter
	glGenBuffers(1, &m_atomicBufferHitCounter);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_atomicBufferHitCounter);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_STATIC_DRAW);

	GLuint* hitData = (GLuint*) glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_WRITE_BIT);
	hitData[0] = 0;

	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);


	// init hit buffer
	int hitBufferSize = m_sampler->number_of_samples;

	glGenBuffers(1, &m_ssboHitBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER , hitBufferSize * sizeof(float), NULL, GL_STATIC_DRAW);

	float *hitBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, hitBufferSize * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	
	for(int i=0; i<hitBufferSize; ++i)
	{
		hitBufferData[i] = -1.0f;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);


	// init hit buffer counter for every vertex
	int hitCountBufferSize = m_vertices.size();

	glGenBuffers(1, &m_ssboHitCountBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitCountBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER , hitCountBufferSize * sizeof(float), NULL, GL_STATIC_DRAW);

	float *hitCountBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, hitCountBufferSize * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	
	for(int i=0; i<hitCountBufferSize; ++i)
	{
		hitCountBufferData[i] = 0.0f;
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);
}

void Scene::buildVBOMesh()
{
	int idx=0;
	for (std::map<uint, MeshVertex>::iterator iterVert=m_vertices.begin(); iterVert!=m_vertices.end(); ++iterVert, ++idx)
	{
		MeshVertex &vert = iterVert->second;
		std::vector<vec3> &transCoeffs = m_transCoeffs[idx];
	
		vec3 res = vec3(0.0f, 0.0f, 0.0f);
		for(int j=0; j<m_numBands * m_numBands; ++j)
		{
			res.x += m_lightCoeffs[j].x * transCoeffs[j].x;
			res.y += m_lightCoeffs[j].y * transCoeffs[j].y;
			res.z += m_lightCoeffs[j].z * transCoeffs[j].z;
		}

		vert.prt = res;
	}

	std::vector<vec3> vertices;
    std::vector<vec3> normals;
	std::vector<vec3> texCoords;
	std::vector<vec3> prts;
	std::vector<float> occlusions;

	for(std::map<uint, MeshFace>::const_iterator iterFaces = m_faces.begin(); iterFaces != m_faces.end(); ++iterFaces)
	{
		const MeshFace &f = iterFaces->second;

		MeshVertex &va = m_vertices.find(f.a)->second;
		MeshVertex &vb = m_vertices.find(f.b)->second;
		MeshVertex &vc = m_vertices.find(f.c)->second;

        vertices.push_back(va.pos);
        vertices.push_back(vb.pos);
        vertices.push_back(vc.pos);

        normals.push_back(va.normal);
        normals.push_back(vb.normal);
        normals.push_back(vc.normal);

		texCoords.push_back(va.texCoords);
		texCoords.push_back(vb.texCoords);
		texCoords.push_back(vc.texCoords);

		prts.push_back(va.prt);
		prts.push_back(vb.prt);
		prts.push_back(vc.prt);

		occlusions.push_back(va.occlusion);
		occlusions.push_back(vb.occlusion);
		occlusions.push_back(vc.occlusion);
	}

	uint nrVertices = vertices.size();
	VertexBufferObjectAttribs::DATA *attrData = new VertexBufferObjectAttribs::DATA[nrVertices];	

	vec3 color = vec3(0.95, 0.95, 0.95);
	
    for(int i=0; i<nrVertices; ++i)
	{
        vec3 p = vertices[i];
        vec3 n = normals[i];
		vec3 uv = texCoords[i];
		vec3 prt = prts[i];
		float o = occlusions[i];

		attrData[i].vx = p.x;
		attrData[i].vy = p.y;
		attrData[i].vz = p.z;
		attrData[i].vw = 1.0f;
			
		attrData[i].nx = n.x;
		attrData[i].ny = n.y;
		attrData[i].nz = n.z;
		attrData[i].nw = 0.0f;
			
		attrData[i].cx = prt.x;
		attrData[i].cy = prt.y;
		attrData[i].cz = prt.z;
		attrData[i].cw = o;
			
		attrData[i].tx = uv.x;
		attrData[i].ty = uv.y;
		attrData[i].tz = 0.0f;
		attrData[i].tw = 0.0f;
	}

	delete m_vboMesh;
	m_vboMesh = new VertexBufferObjectAttribs();
	m_vboMesh->setData(attrData, GL_STATIC_DRAW, nrVertices, GL_TRIANGLES);

    m_vboMesh->addAttrib(VERTEX_POSITION);
    m_vboMesh->addAttrib(VERTEX_NORMAL);
    m_vboMesh->addAttrib(VERTEX_COLOR);
    m_vboMesh->addAttrib(VERTEX_TEXTURE);
    m_vboMesh->bindAttribs();

    delete[] attrData;
}

void Scene::renderWorld(const Transform &trans, const GlobalObjectParam &param)
{
	//debugIsOccluded();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    m_light->render(trans, param);
	renderSkyDome(trans, param);

	glPopClientAttrib();
    glPopAttrib();
}

void Scene::renderObjects(const Transform &trans, const GlobalObjectParam &param)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	
		renderDebug(trans, param);
		renderMesh(trans, param);

	glPopClientAttrib();
    glPopAttrib();
}

void Scene::renderObjectsDepth(const Transform &trans, const GlobalObjectParam &param)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	

	glPopClientAttrib();
    glPopAttrib();
}

void Scene::renderMesh(const Transform &trans, const GlobalObjectParam &param)
{
    mat4 projection = trans.projection;
    mat4 view = trans.view;
    mat4 model = mat4::identitiy();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glDisable(GL_CULL_FACE);

    m_shaderMesh->bind();
        m_shaderMesh->set3f("lightPos", param.lightPos);
		m_shaderMesh->set3f("camPos", param.camPos);

        m_shaderMesh->setMatrix("matProjection", projection, GL_TRUE);
        m_shaderMesh->setMatrix("matView", view, GL_TRUE);
        m_shaderMesh->setMatrix("matModel", model, GL_TRUE); 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texLightProbe);
		m_shaderMesh->seti("texLightProbe", 0);
        
        m_vboMesh->render();    

    m_shaderMesh->release();

	glPopClientAttrib();
    glPopAttrib();

	renderTexture(m_texLightProbe, 10, 10, 300, 300);
}

void Scene::renderSkyDome(const Transform &trans, const GlobalObjectParam &param)
{
    mat4 projection = trans.projection;
    mat4 view = trans.view;
    mat4 model = mat4::identitiy();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glDisable(GL_CULL_FACE);

    m_shaderSkyDome->bind();
        m_shaderSkyDome->set3f("lightPos", param.lightPos);
		m_shaderSkyDome->set3f("camPos", param.camPos);

        m_shaderSkyDome->setMatrix("matProjection", projection, GL_TRUE);
        m_shaderSkyDome->setMatrix("matView", view, GL_TRUE);
        m_shaderSkyDome->setMatrix("matModel", model, GL_TRUE); 

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texLightProbe);
		m_shaderSkyDome->seti("texLightProbe", 0);
        
        m_vboSkyDome->render();    

    m_shaderSkyDome->release();

	glEnable(GL_CULL_FACE);
	glPopClientAttrib();
    glPopAttrib();

	renderTexture(m_texLightProbe, 10, 10, 300, 300);
}

void Scene::renderDebug(const Transform &trans, const GlobalObjectParam &param)
{
    mat4 projection = trans.projection;
    mat4 view = trans.view;
    mat4 model = mat4::identitiy();

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	glEnableFixedFunction(trans);

		
			for(int i=0; i<m_debugSampleDirs.size(); ++i)
			{
				glBegin(GL_LINES);
					vec3 color = m_debugSampleColor[i];
					glColor3f(color.x, color.y, color.z);

					vec3 dir = m_debugSampleDirs[i];
					glVertex3f(m_debugRayOrigin.x, m_debugRayOrigin.y, m_debugRayOrigin.z);
					glVertex3f(m_debugRayOrigin.x + dir.x, m_debugRayOrigin.y + dir.y, m_debugRayOrigin.z + dir.z);
				glEnd();

				float dist = m_debugDistToIntersections[i];
				vec3 tmpDir = normalize(dir) * dist;
				glColor3f(0.0f, 0.0f, 1.0f);
				glPointSize(15.0f);
				glBegin(GL_POINTS);
					glVertex3f(m_debugRayOrigin.x + tmpDir.x, m_debugRayOrigin.y + tmpDir.y, m_debugRayOrigin.z + tmpDir.z);
				glEnd();
			}
		
	glDisableFixedFunction();

	glPopClientAttrib();
    glPopAttrib();

	renderTexture(m_texLightProbe, 10, 10, 300, 300);
}

void Scene::update(float delta)
{
    m_light->update(delta);
}

void Scene::generateSamples()
{
	if(m_sampler)
		delete m_sampler;

	m_sampler = new Sampler();
	m_sampler->samples = new Sample [m_numSamples * m_numSamples];
	m_sampler->number_of_samples = m_numSamples * m_numSamples;

	for (int i = 0; i < m_numSamples; i++)
	{
		for (int j = 0; j < m_numSamples; j++)
		{
			float a = (((float) i) + frand(0.0f, 1.0f)) / (float) m_numSamples;
			float b = (((float) j) + frand(0.0f, 1.0f)) / (float) m_numSamples;
			float theta = 2.0f * acos( sqrt( 1.0f - a));
			float phi = 2.0f * math_pi * b;
			float x = sin(theta)*cos(phi);
			float y = sin(theta)*sin(phi);
			float z = cos(theta);
			int k = i*m_numSamples + j;

			vec3 catesianCoords = normalize((vec3(x,y ,z)));
			
			m_sampler->samples[k].spherical_coord.theta = theta;
			m_sampler->samples[k].spherical_coord.phi = phi;
			m_sampler->samples[k].cartesian_coord.x = catesianCoords.x;
			m_sampler->samples[k].cartesian_coord.y = catesianCoords.y;
			m_sampler->samples[k].cartesian_coord.z = catesianCoords.z;
			m_sampler->samples[k].sh_functions = NULL;
		}
	}
}

void Scene::precomputeSHFunctions()
{
	for (int i = 0; i < m_sampler->number_of_samples; i++)
	{
		float* sh_functions = new float [m_numBands * m_numBands];
		m_sampler->samples[i].sh_functions = sh_functions;

		float theta = m_sampler->samples[i].spherical_coord.theta;
		float phi   = m_sampler->samples[i].spherical_coord.phi;

		for (int l = 0; l < m_numBands; l++)
		{
			for (int m = -l; m <= l; m++)
			{
				int j = l*(l+1) + m;
				sh_functions[j] = SphericalHarmonic(l, m, theta, phi);
			}
		}
	}
}

void Scene::lightProbeAccess(vec3 &color, vec3 direction)
{
	float d = sqrt(direction.x*direction.x + direction.y*direction.y);
	float r = (d == 0) ? 0.0f : (1.0f/math_pi/2.0f) * acos(direction.z) / d;

	float tex_coord [2];
	tex_coord[0] = 0.5f + direction.x * r;
	tex_coord[1] = 0.5f + direction.y * r;

	int pixel_coord [2];
	pixel_coord[0] = tex_coord[0] * m_lightProbeWidth;
	pixel_coord[1] = (1.0-tex_coord[1]) * m_lightProbeHeight;
	int idx = 3 * (pixel_coord[1] * m_lightProbeWidth + pixel_coord[0]);

	color.x = m_lightProbe[idx];
	color.y = m_lightProbe[idx+1];
	color.z = m_lightProbe[idx+2];
}

void Scene::projectLightFunction()
{
	m_lightCoeffs.clear();

	for (int i = 0; i < m_numBands * m_numBands; i++)
	{
		m_lightCoeffs.push_back(vec3(0.0f, 0.0f, 0.0f));
	}

	for (int i = 0; i < m_sampler->number_of_samples; i++)
	{
		vec3& direction = m_sampler->samples[i].cartesian_coord;
		vec3 color;
		lightProbeAccess(color, direction);
	
		//color = vec3(1,1,1);
		for (int j = 0; j < m_numBands * m_numBands; j++)
		{
			float sh_function = m_sampler->samples[i].sh_functions[j];

			m_lightCoeffs[j].x += (color.x * sh_function);
			m_lightCoeffs[j].y += (color.y * sh_function);
			m_lightCoeffs[j].z += (color.z * sh_function);
		}
	}

	float weight = 4.0f * math_pi;
	float scale = weight / m_sampler->number_of_samples;
	for (int i = 0; i < m_numBands * m_numBands; i++)
	{
		m_lightCoeffs[i].x *= scale;
		m_lightCoeffs[i].y *= scale;
		m_lightCoeffs[i].z *= scale;
	}
}

void Scene::projectUnshadowed()
{
	std::cout << std::setprecision(2) << std::fixed;

	m_transCoeffs.clear();

	for (int i = 0; i < m_vertices.size(); i++)
	{	
		std::vector<vec3> coeffs;
		for (int j = 0; j < m_numBands * m_numBands; j++)
		{
			coeffs.push_back(vec3(0.0f, 0.0f, 0.0f));
		}
		m_transCoeffs.push_back(coeffs);
	}
	

	int idx=0;
	for (std::map<uint, MeshVertex>::iterator iterVert=m_vertices.begin(); iterVert!=m_vertices.end(); ++iterVert, ++idx)
	{
		MeshVertex &vert = iterVert->second;
		vec3 n = normalize(vert.normal);
		vec3 albedo = vec3(0.5f, 0.5f, 0.5f);
		
		std::vector<vec3> &coeffs = m_transCoeffs[idx];

		for (int j = 0; j < m_sampler->number_of_samples; j++)
		{
			Sample& sample = m_sampler->samples[j];
			
			float cosine_term = max(0.0f, dot(n, sample.cartesian_coord));

			for (int k = 0; k < m_numBands * m_numBands; k++)
			{
				float sh_function = sample.sh_functions[k];
			
				coeffs[k].x += (albedo.x * sh_function * cosine_term);
				coeffs[k].y += (albedo.y * sh_function * cosine_term);
				coeffs[k].z += (albedo.z * sh_function * cosine_term);
			}
		}

		float p = float(idx) / float(m_vertices.size()-1) * 100.0f;
		std::cout << "Scene::projectUnshadowed(): " << p << "%" << "\r";
	}
	std::cout << std::endl;

	float weight = 4.0f * math_pi;
	float scale = weight / m_sampler->number_of_samples;
	for (int i = 0; i < m_transCoeffs.size(); i++)
	{
		std::vector<vec3> &coeffs = m_transCoeffs[i];

		for (int j = 0; j < m_numBands * m_numBands; j++)
		{
			coeffs[j].x *= scale;
			coeffs[j].y *= scale;
			coeffs[j].z *= scale;
		}
	}
}

void Scene::projectShadowed()
{
	std::cout << std::setprecision(2) << std::fixed;

	m_transCoeffs.clear();

	for (int i = 0; i < m_vertices.size(); i++)
	{	
		std::vector<vec3> coeffs;
		for (int j = 0; j < m_numBands * m_numBands; j++)
		{
			coeffs.push_back(vec3(0.0f, 0.0f, 0.0f));
		}
		m_transCoeffs.push_back(coeffs);
	}
	
	int idx=0;
	for (std::map<uint, MeshVertex>::iterator iterVert=m_vertices.begin(); iterVert!=m_vertices.end(); ++iterVert, ++idx)
	{
		MeshVertex &vert = iterVert->second;
		fillHitBuffer(vert.pos, vert.id);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitBuffer);
		float *hitBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_sampler->number_of_samples *  sizeof(float), GL_MAP_READ_BIT);

		vec3 n = normalize(vert.normal);
		vec3 albedo = vec3(0.5f, 0.5f, 0.5f);
		
		std::vector<vec3> &coeffs = m_transCoeffs[idx];

		for (int j = 0; j < m_sampler->number_of_samples; j++)
		{
			Sample& sample = m_sampler->samples[j];

			if(hitBufferData[j] < 0.0f)
			{
				float cosine_term = max(0.0f, dot(n, sample.cartesian_coord));

				for (int k = 0; k < m_numBands * m_numBands; k++)
				{
					float sh_function = sample.sh_functions[k];

					coeffs[k].x += (albedo.x * sh_function * cosine_term);
					coeffs[k].y += (albedo.y * sh_function * cosine_term);
					coeffs[k].z += (albedo.z * sh_function * cosine_term);
				}
			}
		}

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);

		float p = float(idx) / float(m_vertices.size()-1) * 100.0f;
		std::cout << "Scene::projectShadowed(): "<< p << "%" << "\r";
	}

	float weight = 4.0f * math_pi;
	float scale = weight / m_sampler->number_of_samples;
	for (int i = 0; i < m_transCoeffs.size(); i++)
	{
		std::vector<vec3> &coeffs = m_transCoeffs[i];

		for (int j = 0; j < m_numBands * m_numBands; j++)
		{
			coeffs[j].x *= scale;
			coeffs[j].y *= scale;
			coeffs[j].z *= scale;
		}
	}
}

bool Scene::visibility(int vertIdx, const vec3 &dir)
{
	bool visible = true;

	vec3 &p = m_vertices.find(vertIdx)->second.pos;

	for (std::map<uint, MeshFace>::iterator iterFaces=m_faces.begin(); iterFaces!=m_faces.end(); ++iterFaces)
	{
		MeshFace &f = iterFaces->second;

		if ((vertIdx != f.a) && (vertIdx != f.b) && (vertIdx != f.c))
		{
			MeshVertex &va = m_vertices.find(f.a)->second;
			MeshVertex &vb = m_vertices.find(f.b)->second;
			MeshVertex &vc = m_vertices.find(f.c)->second;
			
			float dist = 0.0f;
			visible = !intersectTriangle(p, dir, va.pos, vb.pos, vc.pos, dist);
			//visible = !rayIntersectsTriangle(p, dir, va.pos, vb.pos, vc.pos);

			if (!visible)
				break;
		}
	}
	return(visible);
}

bool Scene::visibility(const vec3 &rayOrigin, const vec3 &dir, float &dist)
{
	bool visible = true;

	for (std::map<uint, MeshFace>::iterator iterFaces=m_faces.begin(); iterFaces!=m_faces.end(); ++iterFaces)
	{
		MeshFace &f = iterFaces->second;

		MeshVertex &va = m_vertices.find(f.a)->second;
		MeshVertex &vb = m_vertices.find(f.b)->second;
		MeshVertex &vc = m_vertices.find(f.c)->second;

		visible = !rayIntersectsTriangle(rayOrigin, dir, va.pos, vb.pos, vc.pos, dist);
		
		if (!visible)
			break;
	}

	return(visible);
}

bool Scene::visibilityGPU(const vec3 &p, const vec3 &dir, float& dist)
{
	// first rest hit counter
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_atomicBufferHitCounter);

	GLuint *counter = (GLuint*) glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint),  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
	counter[0] = 0;

	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssboFaceData);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssboIntersectionResult);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, m_atomicBufferHitCounter);

	int num_groups_x = m_faces.size() / m_numLocalWorkGroups;

	m_shaderRayIntersection->bind();
	m_shaderRayIntersection->set3f("p", p);
	m_shaderRayIntersection->set3f("dir", dir);

		glDispatchCompute(num_groups_x, 1, 1);
		glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT) ;
	m_shaderRayIntersection->release();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, 0);

	// grab result

	bool isVisible = true;
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, m_atomicBufferHitCounter);
		counter = (GLuint*)glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT);

	if(counter[0] > 0)
		isVisible = false;
	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

	//glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	//// grab result
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboIntersectionResult);
	//float *resData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 8 * sizeof(float), GL_MAP_WRITE_BIT);

	////qDebug() << "GPU: " << resData[0] << resData[1] << resData[2] << resData[3] <<" | " << resData[4] << resData[5] << resData[6] << resData[7];

	//bool isVisible = true;

	//if(resData[0] == 1 )// we have a hit
	//{
	//	dist = resData[1];
	//	isVisible = false;
	//}

	//if(resData[4] == 1) // we have a hit
	//{
	//	dist = resData[5];
	//	isVisible = false;
	//}

	//glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	//glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);

	return isVisible;
}

void Scene::fillHitBuffer(vec3 vertPos, int vertId)
{
	// rest hit buffer
	float d = -1;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitBuffer);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R32F, GL_RED, GL_FLOAT, &d);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);

	// fill hit buffer 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_ssboFaceData);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ssboSampleDirs);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_ssboHitBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, m_ssboHitCountBuffer);
	
	int local_work_size_x = 39;
	int local_work_size_y = 39;
	int local_work_size_z = 1;

	int num_groups_x = ceil(float(m_faces.size()) / float(local_work_size_x)); // / m_numLocalWorkGroups;
	int num_groups_y = ceil(float(m_sampler->number_of_samples) / float(local_work_size_y) );
	int num_groups_z = 1.0;

//	qDebug() << "NumGroups: " << num_groups_x << num_groups_y << num_groups_z;

	m_shaderRayIntersection->bind();
	m_shaderRayIntersection->set3f("rayOrigin", vertPos);
	m_shaderRayIntersection->seti("rayOriginID", vertId);
		glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
		//glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT) ;
	m_shaderRayIntersection->release();

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
}

void Scene::debugIsOccluded()
{
	m_debugRayOrigin = vec3(0.0f, 1.0f, 0.0);
	fillHitBuffer(m_debugRayOrigin, -1);

	int hitBufferSize = m_sampler->number_of_samples;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitBuffer);
	float *hitBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, hitBufferSize *  sizeof(float), GL_MAP_READ_BIT);
	//
	//for(int i=0; i<hitBufferSize; ++i)
	//{
	//	qDebug() << "hitBuffer: " << hitBufferData[i];
	//}

	for(int i=0; i<m_sampler->number_of_samples; ++i)
	{
		Sample &samp = m_sampler->samples[i];
		vec3 dir = samp.cartesian_coord;

		float dist = hitBufferData[i];

		vec3 col = vec3(0.0f, 1.0f, 0.0f);
		if(dist < 0.0f)
		{
			col = vec3(1.0f, 0.0f, 0.0f);
			dist = 0.0f;
		}
		//else
		{
		
			m_debugSampleDirs.push_back(normalize(dir)*2);
			m_debugSampleColor.push_back(col);
			m_debugDistToIntersections.push_back(dist);
		}
	}

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);
}

bool Scene::intersectTriangle(vec3 rayStart, vec3 rayDir, vec3 v0, vec3 v1, vec3 v2, float &t)
{
	float eps = 0.000000001f;

	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;

	vec3 p = cross(rayDir, e2);
	float a = dot(e1, p);

	if(a > -eps && a < eps)
	{
		return false;
	}

	float f = 1.0f / a;
	vec3 s = rayStart - v0;
	float u = f * dot(s,p);

	if(u < 0.0f || u > 1.0f)
		return false;

	vec3 q = cross(s, e1);
	float v = f * dot(rayDir, q);

	if(v < 0.0f || u+v > 1.0f)
		return false;

	t = f * dot(e2, q);

	return true;
}

void Scene::ambientOcclusion()
{
	std::cout << std::setprecision(2) << std::fixed;
	//// first collect occlusion data
	//for(std::map<uint, MeshVertex>::iterator iterVert=m_vertices.begin(); iterVert!=m_vertices.end(); ++iterVert)
	//{
	//	MeshVertex &vert = iterVert->second;
	//	fillHitBuffer(vert.pos, vert.id);
	//}

	//glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitCountBuffer);
	//float *hitCountBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_vertices.size() *  sizeof(float), GL_MAP_READ_BIT);

	int count = 0;
	for(std::map<uint, MeshVertex>::iterator iterVert=m_vertices.begin(); iterVert!=m_vertices.end(); ++iterVert, ++count)
	{
		MeshVertex &vert = iterVert->second;
		fillHitBuffer(vert.pos, vert.id);

		glBindBuffer(GL_SHADER_STORAGE_BUFFER , m_ssboHitBuffer);
		float *hitBufferData = (float*) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_sampler->number_of_samples *  sizeof(float), GL_MAP_READ_BIT);

		int numHits = 0; //hitCountBufferData[vert.id];
		for(int i=0; i<m_sampler->number_of_samples; ++i)
		{
			float dist = hitBufferData[i];
			if(dist > -1)
				++numHits;
		}

		float p = float(count) / float(m_vertices.size()-1) * 100.0f;
		std::cout << "Scene::ambientOcclusion(): " << p << "%" << "\r";

		vert.occlusion = (float) numHits / (float)m_sampler->number_of_samples;
		//qDebug() << vert.occlusion;
	
		//if(vert.id == 10)
	//	qDebug() << vert.id <<  "occ: " << numHits;
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER , 0);
	}

}