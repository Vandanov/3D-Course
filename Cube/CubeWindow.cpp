//
// Created by Sergey Vandanov on 2/16/21.
//

#include "CubeWindow.h"

#include <QOpenGLFunctions>
#include <QScreen>

#include <array>
#include <iostream>
#include <QKeyEvent>
#include <QCursor>
#include <QPainter>

namespace {
    constexpr std::array<GLint, 14> gElement = {
            4, 0, 6, 2, 3, 0, 1, 4, 5, 6, 7, 3, 5, 1
    };
    constexpr std::array<GLfloat, 12> gLine = {
            0.0f, 0.0f,
            0.0f, 5.0f,
            0.0f, 0.0f,
            5.0f, 0.0f,
            0.0f, 0.0f,
            -5.0f * 0.5f, -0.866f * 5.0f

    };
    constexpr std::array<GLfloat, 18> gLineColor = {
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };
    constexpr std::array<GLfloat, 18> gContour = {
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            5.0f, 0.0f,
            0.0f, 0.0f,
            -5.0f * 0.5f, -0.866f * 5.0f
    };

    constexpr unsigned int DIMENSION_SIZE = 3;
    constexpr int MAX_ROTATION_SPEED = 250;
}

namespace fgl {
    void CubeWindow::init() {

        initSlider_();

        fillBasisArray();
        fillVerticesArray();

        qColorDialog_.setGeometry(0, 0, 300, 300);
        fillColorArray(qColorDialog_.currentColor());
        //TODO: cube class, axis, and window which link all different type
        program_ = std::make_unique<QOpenGLShaderProgram>(this);
        program_->addShaderFromSourceFile(QOpenGLShader::Vertex,
                                          ":/Shaders/cube.vs");
        program_->addShaderFromSourceFile(QOpenGLShader::Fragment,
                                          ":/Shaders/cube.fs");
        program_->link();
        posAttr_ = program_->attributeLocation("posAttr");
        assert(posAttr_ != -1);
        colAttr_ = program_->attributeLocation("colAttr");
        assert(colAttr_ != -1);
        matrixUniform_ = program_->uniformLocation("matrix");
        assert(matrixUniform_ != -1);
    }

    void CubeWindow::initSlider_() {
        speedRotSlider_.setOrientation(Qt::Orientation::Horizontal);
        speedRotSlider_.setGeometry(0, 0, 150, 40);
        speedRotSlider_.setTickInterval(25);
        speedRotSlider_.setTickPosition(static_cast<QSlider::TickPosition>(25));
        speedRotSlider_.setValue(50);
        speedRotSlider_.setMaximum(MAX_ROTATION_SPEED);

        zRotSlider_.setOrientation(Qt::Orientation::Horizontal);
        zRotSlider_.setGeometry(0, 0, 150, 40);
        zRotSlider_.setMaximum(400);

    }

    void CubeWindow::keyPressEvent(QKeyEvent *event) {
        if (event->key() == Qt::Key_C && !isColorPanel_) {
            qColorDialog_.show();
            isColorPanel_ = true;
        } else if (event->key() == Qt::Key_C && isColorPanel_) {
            qColorDialog_.hide();
            isColorPanel_ = false;
        } else if (event->key() == Qt::Key_M) {
            mousePos_ = {
                    QCursor::pos().x(), QCursor::pos().y()
            };
        } else if (event->key() == Qt::Key_S && isSlider_) {
            speedRotSlider_.hide();
            zRotSlider_.hide();
            isSlider_ = false;
        } else if (event->key() == Qt::Key_S && !isSlider_) {
            speedRotSlider_.show();
            zRotSlider_.show();
            isSlider_ = true;
        }
    }

    void CubeWindow::render() {
        const auto retinaScale = devicePixelRatio();
        glViewport(0, 0, width() * retinaScale, height() * retinaScale);

        if (isColorPanel_ && qColorDialog_.isEnabled()) {
            fillColorArray(qColorDialog_.currentColor());
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        program_->bind();

        drawCube();
//        drawContour();
        drawAxis();

        program_->release();
        ++frame_;
    }

    void CubeWindow::drawCube() {
        renewMatrix(static_cast<GLfloat>(speedRotSlider_.value() * frame_ / screen()->refreshRate()));
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glVertexAttribPointer(posAttr_, 3,
                              GL_FLOAT, GL_FALSE, 0, gVertices.data());
        glVertexAttribPointer(colAttr_, 3,
                              GL_FLOAT, GL_FALSE, 0, gColors.data());

        glEnableVertexAttribArray(posAttr_);
        glEnableVertexAttribArray(colAttr_);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, gElement.size());

        glDisableVertexAttribArray(colAttr_);
        glDisableVertexAttribArray(posAttr_);
        glDisable(GL_DEPTH_TEST);
    }

    void CubeWindow::drawAxis() {
        renewMatrix(0);

        glVertexAttribPointer(posAttr_, 2,
                              GL_FLOAT, GL_FALSE, 0, gLine.data());
        glVertexAttribPointer(colAttr_, 3,
                              GL_FLOAT, GL_FALSE, 0, gLineColor.data());

        glEnableVertexAttribArray(posAttr_);
        glEnableVertexAttribArray(colAttr_);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glDrawArrays(GL_LINES, 0, 6);
        glDisable(GL_LINE_SMOOTH);
        glDisableVertexAttribArray(colAttr_);
        glDisableVertexAttribArray(posAttr_);
    }

    void CubeWindow::drawContour() {
        renewMatrix(static_cast<GLfloat>(speedRotSlider_.value() * frame_ / screen()->refreshRate()));

        glVertexAttribPointer(posAttr_, 2,
                              GL_FLOAT, GL_FALSE, 0, gContour.data());
        glVertexAttribPointer(colAttr_, 3,
                              GL_FLOAT, GL_FALSE, 0, gLineColor.data());

        glEnableVertexAttribArray(posAttr_);
        glEnableVertexAttribArray(colAttr_);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        glDrawArrays(GL_LINES, 0, 15);
        glDisable(GL_LINE_SMOOTH);
        glDisableVertexAttribArray(colAttr_);
        glDisableVertexAttribArray(posAttr_);
    }


    void CubeWindow::renewMatrix(GLfloat rotationSpeed) {
        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, -15);
        matrix.rotate(rotationSpeed,
                      static_cast<GLfloat>(mousePos_.x()),
                      static_cast<GLfloat>(mousePos_.y()), zRotSlider_.value());
        program_->setUniformValue(matrixUniform_, matrix);
    }

    void CubeWindow::fillColorArray(const QColor &color) { //is it possible to use only 1 row?
        auto red = color.redF();
        auto green = color.greenF();
        auto blue = color.blueF();
        for (int i = 0; i < gColors.size(); i += DIMENSION_SIZE) {
//            if (i % 5 == 0) {
//                auto tmp = red;
//                std::swap(red, green);
//                std::swap(blue, tmp);
//            }
            gColors[i] = red;
            gColors[i + 1] = green;
            gColors[i + 2] = blue;
        }
    }

    void CubeWindow::fillBasisArray() {
        auto startBit = 0;
        for (int i = 0; i < 8 * DIMENSION_SIZE; i += DIMENSION_SIZE) {
            auto binary = std::bitset<3>(startBit).to_string();
            gBasis[i] = static_cast<float>(1 * std::pow(-1, binary[0]));
            gBasis[i + 1] = static_cast<float>(1 * std::pow(-1, binary[1]));
            gBasis[i + 2] = static_cast<float>(1 * std::pow(-1, binary[2]));
            startBit++;
        }
    }

    void CubeWindow::fillVerticesArray() {
        auto elIter = gElement.begin();
        for (int i = 0; i < DIMENSION_SIZE * gElement.size(); i += 3) {
            gVertices[i] = gBasis.at(*elIter * 3);
            gVertices[i + 1] = gBasis[*elIter * 3 + 1];
            gVertices[i + 2] = gBasis[*elIter * 3 + 2];
            elIter++;
        }
    }


} // namespace fgl