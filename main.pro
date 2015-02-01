INCLUDEPATH += E:/Libraries/glew-1.9.0-win32/include \
               E:/Libraries/glut-3.7.6-bin \
               
QMAKE_LIBDIR += E:/Libraries/glew-1.9.0-win32/lib \
                E:/Libraries/glut-3.7.6-bin \

HEADERS +=  Src/*.h \			

SOURCES +=  main.cpp \
            Src/*.cpp \
			           
QT += opengl

LIBS += glew32.lib \
        glut32.lib \

CONFIG += qt debug embed_manifest_exe console 

QMAKE_CXXFLAGS += -MP8
QMAKE_LFLAGS += -LARGEADDRESSAWARE

TEMPLATE += vcapp

RESOURCES = main.qrc
