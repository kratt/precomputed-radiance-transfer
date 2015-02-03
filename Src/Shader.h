#ifndef SHADER
#define SHADER

#include "Global.h"
#include "Misc.h"
#include <QDebug.h>
#include <stdlib.h> 
#include <stdio.h>
#include <QFile>

class Shader : QObject
{
    Q_OBJECT

	private:		
        GLuint m_id;

		const char *m_vFileName;
        const char *m_cFileName;
        const char *m_eFileName;
		const char *m_gFileName;
		const char *m_fFileName;

		const char *m_compFileName;


        QTimer    *m_timer;
        QDateTime m_vOldDateTime;
        QDateTime m_cOldDateTime;
        QDateTime m_eOldDateTime;
        QDateTime m_gOldDateTime;
        QDateTime m_fOldDateTime;
		QDateTime m_compOldDateTime;
        bool      m_firstUpdate;        

        unsigned int m_vertProg;
        unsigned int m_contProg;
        unsigned int m_evalProg;
        unsigned int m_geomProg;
        unsigned int m_fragProg;
		unsigned int m_compProg;
        
        unsigned int m_refreshTime;

		const char *readFile(const char *fileName);
        void checkFile(const char *fileName, QDateTime &oldDateTime, unsigned int type);        
        unsigned int compile(const char *source, unsigned int type);
        void cleanUp();        

    public:       
        enum Config
        {
            AUTO_LOAD = 0,
            MANUAL_LOAD
        };

        Shader(Config conf = AUTO_LOAD);
        Shader(const char *vFileName, const char *fFileName, Config conf = AUTO_LOAD);
        Shader(const char *vFileName, const char *gFileName, const char *fFileName, Config conf = AUTO_LOAD);
        virtual ~Shader();

		void bind() const;
		void release() const;  
        void link() const;

        void attachVertexShader(const char *fileName);
        void attachControlShader(const char *fileName);
        void attachEvaluationShader(const char *fileName);
        void attachGeometryShader(const char *fileName);
        void attachFragmentShader(const char *fileName);   
		void attachComputeShader(const char *fileName);   

        void attachDefault();
        void attachShaderFromSource(const char *source, unsigned int type);

        void setUpdateStatus(Config conf);

        void bindAttribLocation(const char *label, GLuint attribID);


        void seti(const char* label, int arg);
        void setf(const char* label, float arg);
        void set2i(const char* label, int arg1, int arg2);
        void set2f(const char* label, float arg1, float arg2);
        void set3i(const char* label, int arg1, int arg2, int arg3);
        void set3f(const char* label, float arg1, float arg2, float arg3);
        void set3f(const char* label, const vec3 &v);
        void set4i(const char* label, int arg1, int arg2, int arg3, int arg4);
        void set4f(const char* label, float arg1, float arg2, float arg3, float arg4);
        void set3iv(const char* label, const int* args);
        void set3fv(const char* label, const float* args);
		void set4fv(const char* label, const float* args);
        void set4f(const char* label, const vec4 &v);
        void setMatrix(const char* label, const float* m, bool transpose = false);
        void setMatrix(const char* label, const double* m, bool transpose = false);
        void setMatrix(const char* label, const mat4 &mat, bool transpose = false);
        void setMatrix(const char* label, const mat3 &mat, bool transpose = false);

        GLuint id() const;

		void bindAttribLocations();

    public slots:
        void autoUpdate();

};

#endif

