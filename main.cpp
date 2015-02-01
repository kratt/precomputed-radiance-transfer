#include <QApplication>
#include "Src/GLWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QGLFormat format;
    format.setSampleBuffers(true);
    format.setDoubleBuffer(true);
    format.setRgba(true);
    format.setDirectRendering(true);
    format.setSamples(8); 

    QGLContext *context = new QGLContext(format);

    GLWidget widget(context, 1280, 720);
    
    widget.setWindowTitle("");
    widget.show();    
   
    return app.exec();
}
