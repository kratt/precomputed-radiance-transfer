#include "Misc.h"
#include "ModelLoaderObj.h"
#include "VertexBufferObjectAttribs.h"

#include <algorithm>

void glEnable2D(void)
{
	GLint iViewport[4];

	// Get a copy of the viewport
	glGetIntegerv( GL_VIEWPORT, iViewport );

	// Save a copy of the projection matrix so that we can restore it 
	// when it's time to do 3D rendering again.
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();

	// Set up the orthographic projection
	glOrtho( iViewport[0], iViewport[0]+iViewport[2], iViewport[1]+iViewport[3], iViewport[1], -1, 1 );
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();    

	// Make sure depth testing and lighting are disabled for 2D rendering until
	// we are finished rendering in 2D
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );
}

void glDisable2D(void)
{
	glPopAttrib();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
}

void glEnableFixedFunction(const Transform &trans)
{
    const mat4 projT = transpose(trans.projection);
    const mat4 viewT = transpose(trans.view);

    float pm[16];
    float mvm[16];

    projT.data(pm);
    viewT.data(mvm);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(pm);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(mvm);
}

void glDisableFixedFunction()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float frand(float low, float high)
{
    return rand() / (static_cast<float>(RAND_MAX) + 1.0) * (high - low) + low;
}

int irand(int low, int high)
{
    return rand() / (static_cast<float>(RAND_MAX) + 1.0) * (high - low) + low;
}

float lerp(float a, float b, float s)
{
    float c;

    c = (b * s) + (a*(1-s));

    return c;
}

vec3  lerp(const vec3 &a, const vec3 &b, float s)
{
    vec3 c;

    c.x = (b.x * s) + (a.x*(1-s));
    c.y = (b.y * s) + (a.y*(1-s));
    c.z = (b.z * s) + (a.z*(1-s));

    return c;
}

float cosineInterpolation(float a, double b, double s)
{
   float s2;

   s2 = (float)( 1 - cos(s * math_pi) ) / 2;

   return float((a * (1 - s2) + b * s2));
}

double hermiteInterpolation(double y0, double y1, double y2, double y3, double mu, double tension, double bias)
{
    double m0,m1,mu2,mu3;
    double a0,a1,a2,a3;

    mu2 = mu * mu;
    mu3 = mu2 * mu;
    m0  = (y1-y0)*(1+bias)*(1-tension)/2;
    m0 += (y2-y1)*(1-bias)*(1-tension)/2;
    m1  = (y2-y1)*(1+bias)*(1-tension)/2;
    m1 += (y3-y2)*(1-bias)*(1-tension)/2;
    a0 =  2*mu3 - 3*mu2 + 1;
    a1 =    mu3 - 2*mu2 + mu;
    a2 =    mu3 -   mu2;
    a3 = -2*mu3 + 3*mu2;

    return(a0*y1+a1*m0+a2*m1+a3*y2);
}

void renderTexture(uint texture, uint posX, uint posY, float width, float height)
{   
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glColor4f(0.0, 0.0, 0.0, 1.0f);  
    
    glEnable(GL_TEXTURE_2D);     
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);


    glDisable(GL_DEPTH_TEST);
    
    
    glEnable2D();
    glPushMatrix();            
        glTranslatef(posX, posY, 0.0f);       
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(width, 0.0f, 0.0f);
            
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(width, height, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.0, height, 0.0f);
        glEnd();
    glPopMatrix();
    glDisable2D();

    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);   
}

void renderQuad(float size, float r, float g, float b, float a)
{
    float w = size;
    float h = size;
    float d = 0.0;

    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex3f(-w, -h, -d);
        glVertex3f( w, -h, -d);
        glVertex3f( w,  h, -d);
        glVertex3f(-w,  h, -d);
    glEnd();
}

void renderQuad(float width, float height, float r, float g, float b, float a)
{
    float w = width;
    float h = height;
    float d = 0.0;

    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex3f(-w, -h, -d);
        glVertex3f( w, -h, -d);
        glVertex3f( w,  h, -d);
        glVertex3f(-w,  h, -d);
    glEnd();
}

void renderQuad(float posX, float posY, float width, float height)
{
    glPushMatrix();            
        glTranslatef(posX, posY, 0.0f);       
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);

            glTexCoord2f(1.0f, 1.0f);
            glVertex3f(width, 0.0f, 0.0f);
            
            glTexCoord2f(1.0f, 0.0f);
            glVertex3f(width, height, 0.0f);

            glTexCoord2f(0.0f, 0.0f);
            glVertex3f(0.0, height, 0.0f);
        glEnd();
    glPopMatrix();
}

void renderOrigin(float lineWidth)
{
    float size = 0.5;

    glLineWidth(lineWidth);
    glPushMatrix();
        glBegin(GL_LINES);
            glColor3f(1.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(size, 0.0f, 0.0f);

            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, size, 0.0f);

            glColor3f(0.0f, 0.0f, 1.0f);
            glVertex3f(0.0f, 0.0f, 0.0f);
            glVertex3f(0.0f, 0.0f, size);
        glEnd();
    glPopMatrix(); 
    glLineWidth(1.0f);
}

void screenSizeQuad(float width, float height, float fov)
{
    //Screen Size Quad in 3D
    float phi = fov * 0.5;
    float d = 1.0;

    float h = 2 * tan(((double) phi)/180.0 * 3.141592654) * d;
    float w = h * width/height;

    w *= 0.5;
    h *= 0.5;    

    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-w, -h, -d);

        glTexCoord2f(1.0, 0.0);
        glVertex3f( w, -h, -d);
        
        glTexCoord2f(1.0, 1.0);
        glVertex3f( w,  h, -d);

        glTexCoord2f(0.0, 1.0);
        glVertex3f(-w,  h, -d);
    glEnd(); 
}

void renderString(const char *str, int x, int y, Vector4 &color, void *font)
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glEnable2D();

        glColor4f(color.x, color.y, color.z, color.w);
        glRasterPos2i(x, y);

        while(*str)
        {
            glutBitmapCharacter(font, *str);
            ++str;
        }

    glDisable2D();

    glPopClientAttrib();
    glPopAttrib();
}

void renderString(const char *str, int x, int y, float r, float g, float b, float a, void *font)
{
    renderString(str, x, y, vec4(r, g, b, a), font);
}

void smoothBackground(vec4 top, vec4 bottom, float windowWidth, float windowHeight)
{
    glEnable2D();        
    glBegin(GL_QUADS);
        glColor4f(top.x, top.y, top.z, top.w);
        glVertex2f(windowWidth, 0.0);
        glVertex2f(0.0, 0.0);        

        glColor4f(bottom.x, bottom.y, bottom.z, bottom.w);
        glVertex2f(0.0, windowHeight);        
        glVertex2f(windowWidth, windowHeight);        
    glEnd(); 
    glDisable2D();
}

void saveFrameBuffer(QGLWidget *widget)
{
    QImage img = widget->grabFrameBuffer(true);

    QDate date = QDate::currentDate(); 
    QTime time = QTime::currentTime();

    int year = date.year();
    int month = date.month();
    int day = date.day();

    int hour = time.hour();
    int minute = time.minute();
    int second = time.second();

    QString sYear   = QString::number(year);
    QString sMonth  = QString::number(month);
    QString sDay    = QString::number(day);
    QString sHour   = QString::number(hour);
    QString sMinute = QString::number(minute);
    QString sSecond = QString::number(second);
    QString sNull   = QString::number(0);

    QString fMonth  = month < 10 ? sNull + sMonth : sMonth;
    QString fDay    = day < 10 ? sNull + sDay : sDay;
    QString fHour   = hour < 10 ? sNull + sHour : sHour;
    QString fMinute = minute < 10 ? sNull + sMinute : sMinute;
    QString fSecond = second < 10 ? sNull + sSecond : sSecond;

    QString fileName = sYear + fMonth + fDay + "_" + fHour + fMinute + fSecond + ".png";

    img.save(fileName);
}

void saveFrameBuffer(QGLWidget *widget, int idx)
{
    QImage img = widget->grabFrameBuffer(true);

    QDate date = QDate::currentDate(); 
    QTime time = QTime::currentTime();

    int year = date.year();
    int month = date.month();
    int day = date.day();

    int hour = time.hour();
    int minute = time.minute();
    int second = time.second();

    QString number = QString("%1").arg(idx, 5, 10, QChar('0'));

    QString sYear   = QString::number(year);
    QString sMonth  = QString::number(month);
    QString sDay    = QString::number(day);
    QString sHour   = QString::number(hour);
    QString sMinute = QString::number(minute);
    QString sSecond = QString::number(second);
    QString sNull   = QString::number(0);

    QString fMonth  = month < 10 ? sNull + sMonth : sMonth;
    QString fDay    = day < 10 ? sNull + sDay : sDay;
    QString fHour   = hour < 10 ? sNull + sHour : sHour;
    QString fMinute = minute < 10 ? sNull + sMinute : sMinute;
    QString fSecond = second < 10 ? sNull + sSecond : sSecond;

    //QString fileName = "Output/" + sYear + fMonth + fDay + "_" + fHour + fMinute + fSecond + "_" + number + ".png";
    QString fileName = "Output/" + number + ".png";

    img.save(fileName);
}

void getCameraFrame(const Transform &trans, vec3 &dir, vec3 &up, vec3 &right, vec3 &pos)
{
    Matrix4x4 view = trans.view;

    up = normalize(Vector3(view.a21, view.a22, view.a23));
    right = normalize(Vector3(view.a11, view.a12, view.a13));
    dir = normalize(cross(up, right));

    pos.x = -(view.a11 * view.a41 + view.a12 * view.a42 + view.a13 * view.a43);
    pos.y = -(view.a21 * view.a41 + view.a22 * view.a42 + view.a23 * view.a43);
    pos.z = -(view.a31 * view.a41 + view.a32 * view.a42 + view.a33 * view.a43);
}

vec3 getCamPosFromModelView(const Transform &trans)
{
	mat4 m = trans.view;
	vec3 c;

	mat4 inv = m.inverse();

	/*c.x = -(m.a11 * m.a43 + m.a21 * m.a24 + m.a31 * m.a34);
	c.y = -(m.a12 * m.a43 + m.a22 * m.a24 + m.a32 * m.a34);
	c.z = -(m.a13 * m.a43 + m.a23 * m.a24 + m.a33 * m.a34);*/

	c.x = inv.a14;
	c.y = inv.a24;
	c.z = inv.a34;

	return c;
}

vec3 getViewDirFromModelView(const Transform &trans)
{
	mat4 m = trans.view;
	vec3 c;

	c.x = -m.a31;
	c.y = -m.a32;
	c.z = -m.a33;

	return normalize(c);
}

vec3 getUpDirFromModelView(const Transform &trans)
{
	mat4 m = trans.view;
	vec3 c;

	c.x = m.a21;
	c.y = m.a22;
	c.z = m.a23;

	return normalize(c);
}

void checkGLError()
{
    GLenum error = glGetError();

    switch(error)
    {
    case GL_NO_ERROR:
        qDebug() << "GL_ERROR: NO_ERROR";
        break;
    case GL_INVALID_ENUM:
        qDebug() << "GL_ERROR: GL_INVALID_ENUM";
        break;
    case GL_INVALID_OPERATION:
        qDebug() << "GL_ERROR: GL_INVALID_OPERATION";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        qDebug() << "GL_ERROR: GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    case GL_OUT_OF_MEMORY:
        qDebug() << "GL_ERROR: GL_OUT_OF_MEMORY";
        break;
    case GL_STACK_UNDERFLOW:
        qDebug() << "GL_ERROR: GL_STACK_UNDERFLOW";
        break;
    case GL_STACK_OVERFLOW:
        qDebug() << "GL_ERROR: GL_STACK_OVERFLOW";
        break;

    }
}

void checkGLVersion()
{
    char *vendor = NULL;
    char *renderer = NULL;
    char *version = NULL;
    char *extentions = NULL;

    vendor = (char*)glGetString(GL_VENDOR);
    renderer = (char*)glGetString(GL_RENDERER);
    version = (char*)glGetString(GL_VERSION);
    extentions = (char*)glGetString(GL_EXTENSIONS);


    qDebug() << vendor;
    qDebug() << renderer;
    qDebug() << version << "\n";

    QString ext(extentions);
    QStringList extList = ext.split(" ");    

    for(int i=0; i<extList.size(); ++i)
    {
        qDebug() << extList.at(i);
    }

    qDebug() << extList.size() << "Extentions listed!";
}

void colorMap(float x, float * out, float * cm)
{
	x = clamp(x*63, 0.f, 63.f - (float)1e-6);
	int idx = int(x);
	float r = fract(x);
	out[0] = cm[idx*3+0]*(1-r) + cm[(idx+1)*3+0]*r;
	out[1] = cm[idx*3+1]*(1-r) + cm[(idx+1)*3+1]*r;
	out[2] = cm[idx*3+2]*(1-r) + cm[(idx+1)*3+2]*r;
}

void colorMapBgr(float x, float * out, float * cm)
{
	x = clamp(x*63, 0.f, 63.f - (float)1e-6);
	int idx = int(x);
	float r = fract(x);
	out[2] = cm[idx*3+0]*(1-r) + cm[(idx+1)*3+0]*r;
	out[1] = cm[idx*3+1]*(1-r) + cm[(idx+1)*3+1]*r;
	out[0] = cm[idx*3+2]*(1-r) + cm[(idx+1)*3+2]*r;
}

vector<VertexBufferObjectAttribs*> loadObj(QString fileName)
{
    vector <VertexBufferObjectAttribs*> vbos;

    ModelOBJ *model = new ModelOBJ();
	std::vector<vec3> tempVertices;	

    qDebug() << "OBSTACLEOBJ::load():" << fileName.toAscii();
    model->import(fileName.toAscii());
    
	uint nrMeshes = model->getNumberOfMeshes();

    const ModelOBJ::Vertex *vb = model->getVertexBuffer();
    const int *tempIdx = model->getIndexBuffer();


    vec3 scale(10, 10, 10);
    vec3 rot(0, 0, 0);

    mat4 rotMatX = mat4::rotate(rot.x, 1.0f, 0.0f, 0.0f);
	mat4 rotMatY = mat4::rotate(rot.y, 0.0f, 1.0f, 0.0f);
    mat4 rotMatZ = mat4::rotate(rot.z, 0.0f, 0.0f, 1.0f);
    mat4 scaleMat = mat4::scale(scale.x, scale.y, scale.z);

	for(uint i = 0; i < nrMeshes; ++i)	
	{
		std::vector<vec3> tempVertices;	
		std::vector<vec3> tmpNormals;
		std::vector<vec3> tmpTexCoords;

		const ModelOBJ::Mesh &objMesh = model->getMesh(i);
		int startIndex = objMesh.startIndex;
		int vertexCount = objMesh.triangleCount * 3;

		for(int j=startIndex; j<startIndex + vertexCount; ++j)
		{
			uint idx = tempIdx[j];
			Vector3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
			Vector3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
			Vector3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);            

			tempVertices.push_back(v);
			tmpNormals.push_back(n);
			tmpTexCoords.push_back(t);
		} 


		VertexBufferObjectAttribs::DATA *data = new VertexBufferObjectAttribs::DATA[tempVertices.size()];

		for(uint i=0; i<tempVertices.size(); ++i)
		{
			VertexBufferObjectAttribs::DATA d;
			vec3 pos = tempVertices[i];
			vec3 n = tmpNormals[i];
			vec3 t = tmpTexCoords[i];

			d.vx = pos.x;
			d.vy = pos.y;
			d.vz = pos.z;
			d.vw = 1.0f;

			d.cx = 1.0f;
			d.cy = 0.0f;
			d.cz = 0.0f;
			d.cw = 1.0;

			d.nx = n.x;
			d.ny = n.y;
			d.nz = n.z;
			d.nw = 1.0f;

			d.tx = t.x;
			d.ty = t.y;
            d.tz = 0.0f;
            d.tw = 1.0f;

			data[i] = d;
		}

		VertexBufferObjectAttribs* vboMesh = new VertexBufferObjectAttribs();
		vboMesh->setData(data, GL_STATIC_DRAW, tempVertices.size(), GL_TRIANGLES); 

		vboMesh->addAttrib(VERTEX_POSITION);
		vboMesh->addAttrib(VERTEX_NORMAL);
		vboMesh->addAttrib(VERTEX_COLOR);
		vboMesh->addAttrib(VERTEX_TEXTURE);
		vboMesh->bindAttribs();

		delete[] data;

        vbos.push_back(vboMesh);
    }

    return vbos;
}


// PRT stuff

float DoubleFactorial(int n)
{
	if (n <= 1)
		return(1);
	else
		return(n * DoubleFactorial(n-2));
}

float Legendre(int l, int m, float x)
{
	float result;
	if (l == m+1)
		result = x*(2*m + 1)*Legendre(m, m, x);
	else if (l == m)
		result = pow(-1.0f, m)*DoubleFactorial(2*m-1)*pow(float(1-x*x), float(m/2.0f));
	else
		result = (x*(2*l-1)*Legendre(l-1, m, x) - (l+m-1)*Legendre(l-2, m, x))/(l-m);
	return(result);
}

float SphericalHarmonic(int l, int m, float theta, float phi)
{
	float result;
	if (m > 0)
		result = sqrt(2.0f) * K(l, m) * cos(m*phi) * Legendre(l, m, cos(theta));
	else if (m < 0)
		result = sqrt(2.0f) * K(l, m) * sin(-m*phi) * Legendre(l, -m, cos(theta));
	else
		result = K(l, 0) * Legendre(l, 0, cos(theta));
	return(result);
}

int factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

float K(int l, int m)
{
	float num = (2*l+1) * factorial(l-abs(m));
	float denom = 4*math_pi * factorial(l+abs(m));
	float result = sqrt(num/denom);
	return(result);
}

void loadObjData(const char *fileName, std::map<uint, MeshFace> &faces, std::map<uint, MeshVertex> &vertices, bool normalizeData)
{
	faces.clear();
	vertices.clear();

	ModelOBJ model;
	model.import(fileName);

	uint nrMeshes = model.getNumberOfMeshes();

	const ModelOBJ::Vertex *vb = model.getVertexBuffer();
	const int *tempIdx = model.getIndexBuffer();

	std::vector<uint> idxs;
	std::vector<vec3> tmpVertices;
	std::vector<vec3> tmpNormals;
	std::vector<vec3> tmpTexCoords;

	int numVertices = 0;


	for(uint i = 0; i < nrMeshes; ++i)	
	{
		const ModelOBJ::Mesh &objMesh = model.getMesh(i);
		int startIndex = objMesh.startIndex;
		int vertexCount = objMesh.triangleCount * 3;

		for(int j=startIndex; j<startIndex + vertexCount; ++j)
		{
			uint idx = tempIdx[j];
			Vector3 v(vb[idx].position[0], vb[idx].position[1], vb[idx].position[2]);
			Vector3 n(vb[idx].normal[0], vb[idx].normal[1], vb[idx].normal[2]);
			Vector3 t(vb[idx].texCoord[0], vb[idx].texCoord[1], 0.0f);   

			tmpVertices.push_back(v);
			tmpNormals.push_back(n);
			tmpTexCoords.push_back(t);
			idxs.push_back(idx);

			++numVertices;
		} 

		uint faceID = 0;
		for(uint j=0; j<idxs.size(); j+=3)
		{
			uint a = idxs[j];
			uint b = idxs[j+1];
			uint c = idxs[j+2];

			MeshFace face;

			face.id = faceID;
			face.a = a;
			face.b = b;
			face.c = c;

			vec3 n = tmpNormals[j]; // normal at i, i+1 and i+2 should be the same (triangle lies in a plane)
			face.n = n;

			faces.insert(std::pair<uint, MeshFace>(face.id, face));

			std::map<uint, MeshVertex>::iterator iterVert = vertices.find(a);

			if(iterVert == vertices.end())
			{
				MeshVertex vert;
				vert.id  = a;
				vert.pos = tmpVertices[j]; 
				vert.normal = tmpNormals[j];
				vert.texCoords = tmpTexCoords[j];
				vert.faceIds.push_back(faceID);
				vertices.insert(std::pair<uint, MeshVertex>(a, vert));
			}
			else
			{
				iterVert->second.faceIds.push_back(faceID);
			}

			iterVert = vertices.find(b);
			if(iterVert == vertices.end())
			{
				MeshVertex vert;
				vert.id  = b;
				vert.pos = tmpVertices[j+1]; 
				vert.normal = tmpNormals[j+1];
				vert.texCoords = tmpTexCoords[j+1];
				vert.faceIds.push_back(faceID);
				vertices.insert(std::pair<uint, MeshVertex>(b, vert));
			}
			else
			{
				iterVert->second.faceIds.push_back(faceID);
			}


			iterVert = vertices.find(c);
			if(iterVert == vertices.end())
			{
				MeshVertex vert;
				vert.id  = c;
				vert.pos = tmpVertices[j+2]; 
				vert.normal = tmpNormals[j+2];
				vert.texCoords = tmpTexCoords[j+2];
				vert.faceIds.push_back(faceID);
				vertices.insert(std::pair<uint, MeshVertex>(c, vert));
			}
			else
			{
				iterVert->second.faceIds.push_back(faceID);
			}

			++faceID;
		}
	}

	

	//for(std::map<uint, MeshVertex>::iterator iterVertA = vertices.begin(); iterVertA != vertices.end(); ++iterVertA)
	//{
	//	for(std::map<uint, MeshVertex>::iterator iterVertB = vertices.begin(); iterVertB != vertices.end(); ++iterVertB)
	//	{
	//		if(iterVertA->second.id != iterVertB->second.id)
	//		{
	//			if(length(iterVertA->second.pos - iterVertB->second.pos) < 0.00001f)
	//			{
	//				qDebug() << iterVertA->second.id << "fail";
	//			}
	//		}
	//	}
	//}


	// set center of faces
	for(std::map<uint, MeshFace>::iterator iterFaces = faces.begin(); iterFaces!=faces.end(); ++iterFaces)
	{
		MeshVertex &va = vertices[iterFaces->second.a];
		MeshVertex &vb = vertices[iterFaces->second.b];
		MeshVertex &vc = vertices[iterFaces->second.c];

		iterFaces->second.center = (va.pos + vb.pos + vc.pos) / 3.0f;
	}

	// set for every vertex the neighbor vertices
	for(std::map<uint, MeshVertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{ 
		MeshVertex &vert = iter->second;
		std::vector<uint> &vertIds = vert.vertIds;
		uint vertId = vert.id;

		const std::vector<uint> &faceIds = vert.faceIds;

		for(int i=0; i<faceIds.size(); ++i)
		{
			const MeshFace &face = faces.find(faceIds[i])->second;

			uint a = face.a;
			uint b = face.b;
			uint c = face.c;

			if(vertId != a && find(vertIds.begin(), vertIds.end(), a) == vertIds.end()) 
				vertIds.push_back(a);
			
			if(vertId != b && find(vertIds.begin(), vertIds.end(), b) == vertIds.end()) 
				vertIds.push_back(b);
			
			if(vertId != c && find(vertIds.begin(), vertIds.end(), c) == vertIds.end()) 
				vertIds.push_back(c);

		}
	}

	// set avg. normal for every vertex
	for(std::map<uint, MeshVertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{ 
		MeshVertex &vert = iter->second;
		vert.normal = vec3(0.0f, 0.0f, 0.0f);		
	}
	
	for(std::map<uint, MeshFace>::iterator iterFaces = faces.begin(); iterFaces!=faces.end(); ++iterFaces)
	{
		MeshVertex &va = vertices[iterFaces->second.a];
		MeshVertex &vb = vertices[iterFaces->second.b];
		MeshVertex &vc = vertices[iterFaces->second.c];
		
		vec3 n = normalize(cross(vb.pos-va.pos, vc.pos-va.pos));
		iterFaces->second.n = n;

		va.normal += n;
		vb.normal += n;
		vc.normal += n;
	}
		
	for(std::map<uint, MeshVertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{ 
		MeshVertex &vert = iter->second;
		vert.normal /= vert.faceIds.size();		
	}

	if(normalizeData)
		normalize(vertices);
}

void normalize(std::map<uint, MeshVertex> &vertices)
{
	// determine min and max extend
	float min_x = math_maxfloat;
	float min_y = math_maxfloat;
	float min_z = math_maxfloat;

	float max_x = math_minfloat;
	float max_y = math_minfloat;
	float max_z = math_minfloat;


	for(std::map<uint, MeshVertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{ 
		const MeshVertex &vert = iter->second;
		vec3 pos = vert.pos;

		if(pos.x < min_x) min_x = pos.x;
		if(pos.y < min_y) min_y = pos.y;
		if(pos.z < min_z) min_z = pos.z;

		if(pos.x > max_x) max_x = pos.x;
		if(pos.y > max_y) max_y = pos.y;
		if(pos.z > max_z) max_z = pos.z;
	}

	vec3 minDims = vec3(min_x, 0, min_z);
	vec3 maxDims = vec3(max_x, 0, max_z);	
	vec3 center = minDims + (maxDims-minDims)/2.0f;

	// find scaling factor
	// find max distance in dims.
	float x_dist = abs(minDims.x - maxDims.x);
	float y_dist = abs(minDims.y - maxDims.y);
	float z_dist = abs(minDims.z - maxDims.z);

	bool scale_x = false;
	bool scale_y = false;
	bool scale_z = false;

	
	float scale_factor = 1.0f;

	scale_factor = 1.0f / x_dist;
	scale_factor = 1.0f / y_dist;
	scale_factor = 1.0f / z_dist;

	// translate and scale all vertices
	for(std::map<uint, MeshVertex>::iterator iter = vertices.begin(); iter != vertices.end(); ++iter)
	{ 
		MeshVertex &vert = iter->second;

		vec3 pos = vert.pos;
		vec3 newPos = (pos - center) *scale_factor;

		vert.pos = newPos;
	}
}


bool rayIntersectsTriangle(const vec3 &p, const vec3 &d, const vec3 &v0, const vec3 &v1, const vec3 &v2, float &dist)
{
	vec3 e1 = v1 - v0;
	vec3 e2 = v2 - v0;

	vec3 h = cross(d, e2);

	float a = dot(e1, h);
	if (a > -0.00001f && a < 0.00001f)
		return false;

	float f = 1.0f / a;

	vec3 s = p - v0;
	float u = f * dot(s, h);

	if (u < 0.0f || u > 1.0f)
		return false;

	
	vec3 q = cross(s, e1);
	float v = f * dot(d, q);

	if (v < 0.0f || u + v > 1.0f)
		return false;

	float t = dot(e2, q)*f;
	if (t < 0.0f)
		return false;

	dist = t;
	return true;
}