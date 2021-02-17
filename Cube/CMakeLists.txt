cmake_minimum_required(VERSION 3.17)
project(ForkGL)

set(CMAKE_CXX_STANDARD 17)
set(GLEW_H /usr/local/Cellar/glew/2.2.0/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.2/include/GLFW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH /usr/local/Cellar/qt/5.15.2)

include_directories(${GLEW_H} ${GLFW_H})

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0/lib/libGLEW.2.2.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.2/lib/libglfw.3.dylib)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

add_executable(ForkGL main.cpp Base/GLWindow.cpp Base/GLWindow.hpp TriangleWindow.hpp TriangleWindow.cpp shaders.qrc CubeWindow.cpp CubeWindow.h)

target_link_libraries(ForkGL "-framework OpenGL")
target_link_libraries(ForkGL "-framework GLUT")
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})