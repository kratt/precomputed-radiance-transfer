//Author: Sören Pirk
//Date: 22.01.2013

#include "Shader.h"

Shader::Shader(Config conf)
: m_vFileName(NULL),
  m_cFileName(NULL),
  m_eFileName(NULL),
  m_gFileName(NULL),
  m_fFileName(NULL),  
  m_vOldDateTime(),
  m_cOldDateTime(),
  m_eOldDateTime(),
  m_gOldDateTime(),
  m_fOldDateTime(),
  m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_refreshTime(1000),
  m_firstUpdate(true)
{  
    if(!m_id)
        m_id = glCreateProgram();

     m_timer = new QTimer();

    if(conf == AUTO_LOAD)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(autoUpdate()));  
        m_timer->start(m_refreshTime);
    }
}

Shader::Shader(const char *vFileName, const char *fFileName, Config conf)
: m_vFileName(NULL),
  m_cFileName(NULL),
  m_eFileName(NULL),
  m_gFileName(NULL),
  m_fFileName(NULL),  
  m_vOldDateTime(),
  m_cOldDateTime(),
  m_eOldDateTime(),
  m_gOldDateTime(),
  m_fOldDateTime(),
  m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_refreshTime(1000),
  m_firstUpdate(true)
{  
    if(!m_id)
        m_id = glCreateProgram();

    attachVertexShader(vFileName);
    attachFragmentShader(fFileName);

    m_timer = new QTimer();

    if(conf == AUTO_LOAD)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(autoUpdate()));
        m_timer->start(m_refreshTime);
    }
}

Shader::Shader(const char *vFileName, const char *gFileName, const char *fFileName, Config conf)
: m_vFileName(NULL),
  m_cFileName(NULL),
  m_eFileName(NULL),
  m_gFileName(NULL),
  m_fFileName(NULL),  
  m_vOldDateTime(),
  m_cOldDateTime(),
  m_eOldDateTime(),
  m_gOldDateTime(),
  m_fOldDateTime(),
  m_id(0),
  m_vertProg(0),
  m_contProg(0),
  m_evalProg(0),
  m_geomProg(0),
  m_fragProg(0),
  m_refreshTime(1000),
  m_firstUpdate(true)
{  
    if(!m_id)
        m_id = glCreateProgram();


    attachVertexShader(vFileName);
    attachGeometryShader(gFileName);
    attachFragmentShader(fFileName);

    m_timer = new QTimer();

    if(conf == AUTO_LOAD)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(autoUpdate()));
        m_timer->start(m_refreshTime);
    }
}

Shader::~Shader()
{
    m_timer->stop();
    delete m_timer;

    cleanUp();
}

void Shader::attachVertexShader(const char *fileName)
{
    if(fileName)
    {
        m_vFileName = fileName;
        const char *source = readFile(fileName);

        if(source)
        {
            m_vertProg = compile(source, GL_VERTEX_SHADER);
            glAttachShader(m_id, m_vertProg);

            glLinkProgram(m_id);            

            qDebug() << "SHADER::attachVertexShader():attached:     " << fileName;
        }
        else
        {
            qDebug() << "SHADER::attachVertexShader():not found: " << fileName;
        }
    }
}

void Shader::attachControlShader(const char *fileName)
{
    if(fileName)
    {
        m_cFileName = fileName;
        const char *source = readFile(fileName);

        if(source)
        {
            m_contProg = compile(source, GL_TESS_CONTROL_SHADER);
            glAttachShader(m_id, m_contProg);

            glLinkProgram(m_id);            

            qDebug() << "SHADER::attachControlShader():attached:    " << fileName;
        }
        else
        {
            qDebug() << "SHADER::attachControlShader():not found: " << fileName;
        }

    }
}

void Shader::attachEvaluationShader(const char *fileName)
{
    if(fileName)
    {
        m_eFileName = fileName;
        const char *source = readFile(fileName);

        if(source)
        {
            m_evalProg = compile(source, GL_TESS_EVALUATION_SHADER);
            glAttachShader(m_id, m_evalProg);

            glLinkProgram(m_id);            

            qDebug() << "SHADER::attachEvaluationShader(): attached:" << fileName;
        }
        else
        {
            qDebug() << "SHADER::attachEvaluationShader(): not found:"  << fileName;
        }

    }
}

void Shader::attachGeometryShader(const char *fileName)
{
    if(fileName)
    {
        m_gFileName = fileName;
        const char *source = readFile(fileName);

        if(source)
        {
            m_geomProg = compile(source, GL_GEOMETRY_SHADER);
            glAttachShader(m_id, m_geomProg);

            glLinkProgram(m_id);

            qDebug() << "SHADER::attachGeometryShader(): attached:  " << fileName;
        }
        else
        {
            qDebug() << "SHADER::attachGeometryShader(): not found: " << fileName;
        }
    }
}

void Shader::attachFragmentShader(const char *fileName)
{
    if(fileName)
    {
        m_fFileName = fileName;
        const char *source = readFile(fileName);

        if(source)
        {
            m_fragProg = compile(source, GL_FRAGMENT_SHADER);
            glAttachShader(m_id, m_fragProg);

            glLinkProgram(m_id);

            qDebug() << "SHADER::attachFragmentShader(): attached:  " << fileName;
        }
        else
        {
            qDebug() << "SHADER::attachFragmentShader(): not found: " << fileName;
        }
    }
}



void Shader::attachDefault()
{
    const char *vertSrc = "#version 400 core                                                             \n"
                          "                                                                              \n"                         
                          "#define VERT_POSITION 0                                                       \n"
                          "#define VERT_NORMAL   1                                                       \n"
                          "#define VERT_COLOR	   2                                                       \n"
                          "#define VERT_TEXTURE  3                                                       \n"
                          "                                                                              \n"
                          "uniform mat4x4 matModel;                                                      \n"
                          "uniform mat4x4 matView;                                                       \n"
                          "uniform mat4x4 matProjection;                                                 \n"
                          "                                                                              \n"
                          "layout(location = VERT_POSITION) in vec4 Position;                            \n"
                          "layout(location = VERT_NORMAL)   in vec4 Normal;                              \n"
                          "layout(location = VERT_COLOR)    in vec4 Color;                               \n"
                          "layout(location = VERT_TEXTURE)  in vec4 Texture;                             \n"
                          "                                                                              \n"
                          "out vec4 VertColor;                                                           \n"
                          "                                                                              \n"
                          "void main()                                                                   \n"
                          "{	                                                                           \n"
	                      "    VertColor    = Color;                           	                       \n"
                          "    gl_Position = matProjection * matView * matModel * vec4(Position.xyz, 1); \n"
                          "}                                                                             \n";

    const char *fragSrc = "#version 400 core           \n"
                          "                            \n"
                          "in vec4 VertColor;          \n"
                          "                            \n"
                          "void main()                 \n"
                          "{                           \n"
                          "   gl_FragColor = VertColor;\n"
                          "}                           \n";

    attachShaderFromSource(vertSrc, GL_VERTEX_SHADER);
    attachShaderFromSource(fragSrc, GL_FRAGMENT_SHADER);
}

void Shader::attachShaderFromSource(const char *source, unsigned int type)
{
    if(source)
    {
        if(type == GL_VERTEX_SHADER)
        {
            m_vertProg = compile(source, type);
            glAttachShader(m_id, m_vertProg);
        }
        
        if(type == GL_TESS_CONTROL_SHADER)
        {
            m_contProg = compile(source, type);
            glAttachShader(m_id, m_contProg);
        }

        if(type == GL_TESS_EVALUATION_SHADER)
        {
            m_evalProg = compile(source, type);
            glAttachShader(m_id, m_evalProg);
        }

        if(type == GL_GEOMETRY_SHADER)
        {
            m_geomProg = compile(source, type);
            glAttachShader(m_id, m_geomProg);
        }

        if(type == GL_FRAGMENT_SHADER)
        {
            m_fragProg = compile(source, type);
            glAttachShader(m_id, m_fragProg);
        }

        glLinkProgram(m_id);            
    }    
}

void Shader::checkFile(const char *fileName, QDateTime &oldDateTime, unsigned int type)
{
    QString fName(fileName);
    QFileInfo fileInfo(fName);
    QDateTime dt = fileInfo.lastModified();   

    if(dt != oldDateTime)
    {
        if(!m_firstUpdate)
        {
            if(type == GL_VERTEX_SHADER)
            {
                glDetachShader(m_id, m_vertProg);
                glDeleteShader(m_vertProg);

                attachVertexShader(fileName);
                qDebug()<< "SHADER::Vertex updated";
            }

            if(type == GL_TESS_CONTROL_SHADER)
            {
                glDetachShader(m_id, m_contProg);
                glDeleteShader(m_contProg);

                attachControlShader(fileName);
                qDebug()<< "SHADER::Control updated";
            }

            if(type == GL_TESS_EVALUATION_SHADER)
            {
                glDetachShader(m_id, m_evalProg);
                glDeleteShader(m_evalProg);

                attachEvaluationShader(fileName);
                qDebug()<< "SHADER::Evaluation updated";
            }

            if(type == GL_GEOMETRY_SHADER)
            {
                glDetachShader(m_id, m_geomProg);
                glDeleteShader(m_geomProg);

                attachGeometryShader(fileName);
                qDebug()<< "SHADER::Geometry updated";
            }

            if(type == GL_FRAGMENT_SHADER)
            {
                glDetachShader(m_id, m_fragProg);
                glDeleteShader(m_fragProg);

                attachFragmentShader(fileName);
                qDebug()<< "SHADER::Fragment updated";
            }

            glLinkProgram(m_id);
        }

        oldDateTime = dt;
    }
}

const char *Shader::readFile(const char *fileName)
{
	FILE *fp = NULL;
	char *content = NULL;
	int count = 0;

	if (fileName) 
    {
        fopen_s(&fp, fileName, "rt");

		if (fp) 
        {      
            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

			if (count > 0) 
            {
				content = new char[count + 1];
				count = fread(content, sizeof(char), count, fp);
				content[count] = '\0';
			}

			fclose(fp);
		}
	}

	return content;

	//QFile file(fileName);
	//const char *source = NULL;

	//if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	//{
	//	QTextStream in(&file);
	//	QString s = in.readAll();

 //       source = s.toAscii().data();

 //       //QString temp(source);
 //       //qDebug() << temp;

	//	file.close();
	//}

	//return source;
}

unsigned int Shader::compile(const char *source, unsigned int type)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    int c = 0;
    
    glGetShaderiv(id, GL_COMPILE_STATUS, &c);

    if(!c)
    {
        char* logstr = new char[2048];
        glGetShaderInfoLog(id, 2048, NULL, logstr);
        printf("SHADER::Error compiling shader %s:\n%s\n", source, logstr);
        delete[] logstr;
        //exit(-1);
        return 0;
    }

    return id;
}

void Shader::cleanUp()
{
    if(m_id)
    {
        if(m_vertProg)
        {
            glDetachShader(m_id, m_vertProg);
            glDeleteShader(m_vertProg);
            m_vertProg = 0;
        }

        if(m_contProg)
        {
            glDetachShader(m_id, m_contProg);
            glDeleteShader(m_contProg);
            m_contProg = 0;
        }

        if(m_evalProg)
        {
            glDetachShader(m_id, m_evalProg);
            glDeleteShader(m_evalProg);
            m_evalProg = 0;
        }

        if(m_geomProg)
        {
            glDetachShader(m_id, m_geomProg);
            glDeleteShader(m_geomProg);
            m_geomProg = 0;
        }

        if(m_fragProg)
        {
            glDetachShader(m_id, m_fragProg);
            glDeleteShader(m_fragProg);
            m_fragProg = 0;
        }

        glDeleteProgram(m_id);
        m_id = 0;
    }
}

void Shader::autoUpdate()
{
    checkFile(m_vFileName, m_vOldDateTime, GL_VERTEX_SHADER);
    checkFile(m_cFileName, m_cOldDateTime, GL_TESS_CONTROL_SHADER);
    checkFile(m_eFileName, m_eOldDateTime, GL_TESS_EVALUATION_SHADER);
    checkFile(m_gFileName, m_gOldDateTime, GL_GEOMETRY_SHADER);
    checkFile(m_fFileName, m_fOldDateTime, GL_FRAGMENT_SHADER);

    m_firstUpdate = false;
}

void Shader::setUpdateStatus(Config conf)
{
    if(conf == AUTO_LOAD)
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(autoUpdate()));        
        m_timer->start(m_refreshTime);
    }
    else
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(autoUpdate()));
    }
}

GLuint Shader::id() const
{
    return m_id;
}

void Shader::bind() const
{
    glUseProgram(m_id);
}

void Shader::release() const
{
    glUseProgram(0);
}

void Shader::link() const
{
    glLinkProgram(m_id);
}

void Shader::bindAttribLocation(const char *label, GLuint attribID)
{    
    glBindAttribLocation(m_id, attribID, label);
    //qDebug() << "SHADER::Attrib:" << label << ":" << glGetAttribLocation(m_id, label);
}

void Shader::seti(const char* label, int arg)
{
    //bind();
    //qDebug() << glGetUniformLocation(m_id, label);
    //if(glGetUniformLocation(m_id, label) == -1)
    //{
    //    qDebug() << glGetUniformLocation(m_id, label) << label << gluErrorString(glGetError());
    //}

    glUniform1i(glGetUniformLocation(m_id, label), arg);
}

void Shader::setf(const char* label, float arg)
{
    //bind();
    glUniform1f(glGetUniformLocation(m_id, label), arg);
}

void Shader::set2i(const char* label, int arg1, int arg2)
{
    //bind();
    glUniform2i(glGetUniformLocation(m_id, label), arg1, arg2);
}

void Shader::set2f(const char* label, float arg1, float arg2)
{
    //bind();
    glUniform2f(glGetUniformLocation(m_id, label), arg1, arg2);
}

void Shader::set3i(const char* label, int arg1, int arg2, int arg3)
{
    //bind();
    glUniform3i(glGetUniformLocation(m_id, label), arg1, arg2, arg3);
}

void Shader::set3f(const char* label, float arg1, float arg2, float arg3)
{
    //bind();
    glUniform3f(glGetUniformLocation(m_id, label), arg1, arg2, arg3);
}

void Shader::set3f(const char* label, const Vector3 &v)
{
    //bind();
    glUniform3f(glGetUniformLocation(m_id, label), v.x, v.y, v.z);
}


void Shader::set4i(const char* label, int arg1, int arg2, int arg3, int arg4)
{
    //bind();
    glUniform4i(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4);
}

void Shader::set4f(const char* label, float arg1, float arg2, float arg3, float arg4)
{
    //bind();
    glUniform4f(glGetUniformLocation(m_id, label), arg1, arg2, arg3, arg4);
}

void Shader::set4f(const char* label, const Vector4 &v)
{
    //bind();
    glUniform4f(glGetUniformLocation(m_id, label), v.x, v.y, v.z, v.w);
}

void Shader::set3iv(const char* label, const int* args)
{
    //bind();
    glUniform3iv(glGetUniformLocation(m_id, label), 1, args);
}

void Shader::set3fv(const char* label, const float* args)
{
    //bind();
    glUniform3fv(glGetUniformLocation(m_id, label), 1, args);
}

void Shader::set4fv(const char* label, const float* args)
{
    //bind();
    glUniform4fv(glGetUniformLocation(m_id, label), 1, args);
}

void Shader::setMatrix(const char* label, const float* m, bool transpose)
{
    //bind();
    glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, m);
}

void Shader::setMatrix(const char* label, const double* m, bool transpose)
{
    //bind();
    float fm[] = {(float)m[0], (float)m[1], (float)m[2], (float)m[3], (float)m[4], (float)m[5], (float)m[6], (float)m[7], (float)m[8], (float)m[9], (float)m[10], (float)m[11], (float)m[12], (float)m[13], (float)m[14], (float)m[15]};
    glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, fm);
}

void Shader::setMatrix(const char* label, const mat4 &mat, bool transpose)
{
    float m[16];
    mat.data(m);

    glUniformMatrix4fv(glGetUniformLocation(m_id, label), 1, transpose, m);
}

void Shader::setMatrix(const char* label, const mat3 &mat, bool transpose)
{
    float m[9];
    mat.data(m);

    glUniformMatrix3fv(glGetUniformLocation(m_id, label), 1, transpose, m);
}

void Shader::bindAttribLocations()
{
    this->bindAttribLocation("Position", VERTEX_POSITION);
    this->bindAttribLocation("Normal", VERTEX_NORMAL);
    this->bindAttribLocation("Color", VERTEX_COLOR);
    this->bindAttribLocation("Texture", VERTEX_TEXTURE);
    this->bindAttribLocation("Temp1", VERTEX_TEMP1);
    this->bindAttribLocation("Temp2", VERTEX_TEMP2);
}