#include <QApplication>
#include <QScreen>
#include <QSurfaceFormat>
#include <iostream>
#include "CubeWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QSurfaceFormat format;
    format.setSamples(16);
    format.setVersion(3, 3);
    format.setDepthBufferSize(24);

    fgl::CubeWindow window;
    window.setFormat(format);
    window.resize(QApplication::primaryScreen()->geometry().width(),
                  QApplication::primaryScreen()->geometry().height());
    window.show();
    window.setAnimated(true);


    return QApplication::exec();
}
