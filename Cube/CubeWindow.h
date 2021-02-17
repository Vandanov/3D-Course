//
// Created by Sergey Vandanov on 2/16/21.
//

#ifndef FORKGL_CUBEWINDOW_H
#define FORKGL_CUBEWINDOW_H

#include "Base/GLWindow.hpp"
#include <QColorDialog>
#include <memory>

#include <QOpenGLShaderProgram>
#include <QSlider>


namespace fgl {
    class CubeWindow final : public GLWindow {
    protected:
        void keyPressEvent(QKeyEvent *event) override;

    public:
        void init() override;

        void render() override;

    private:
        GLint posAttr_ = 0;
        GLint colAttr_ = 0;
        GLint matrixUniform_ = 0;
        bool isColorPanel_ = false;
        bool isSlider_ = false;
        int frame_ = 0;


        std::array<GLfloat, 42u> gVertices;
        std::array<GLfloat, 42u> gColors;
        std::array<GLfloat, 24u> gBasis;

        std::unique_ptr<QOpenGLShaderProgram> program_ = nullptr;
        QSlider speedRotSlider_;
        QSlider zRotSlider_;
        QColorDialog qColorDialog_;
        QPoint mousePos_;

        void initSlider_();

        void fillColorArray(const QColor &color);

        void fillVerticesArray();

        void fillBasisArray();

        void drawCube();

        void drawAxis();

        void drawContour();

        void renewMatrix(GLfloat rotationSpeed);
    };
}


#endif //FORKGL_CUBEWINDOW_H
