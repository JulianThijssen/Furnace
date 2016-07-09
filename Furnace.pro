#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T05:07:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Furnace
TEMPLATE = app

INCLUDEPATH += 3rdparty/assimp/include

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/viewwidget.cpp \
    src/modelloader.cpp \
    src/util/vector3f.cpp \
    src/util/vector2f.cpp \
    src/shaderloader.cpp \
    src/util/matrix4f.cpp \
    src/shader.cpp \
    src/normalgenerator.cpp \
    src/image.cpp \
    src/util/color.cpp \
    src/textureloader.cpp \
    src/util/math.cpp \
    src/cage.cpp \
    src/framebuffer.cpp

HEADERS  += src/mainwindow.h \
    src/viewwidget.h \
    src/modelloader.h \
    src/mesh.h \
    src/util/vector3f.h \
    src/util/vector2f.h \
    src/face.h \
    src/model.h \
    src/shaderloader.h \
    src/shader.h \
    src/util/matrix4f.h \
    src/normalgenerator.h \
    src/image.h \
    src/util/color.h \
    src/textureloader.h \
    src/util/math.h \
    src/cage.h \
    src/framebuffer.h

FORMS    += src/mainwindow.ui

LIBS += -L"../3rdparty/assimp/lib" -lassimp
