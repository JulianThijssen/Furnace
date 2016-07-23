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

RESOURCES = furnace.qrc

SOURCES += src/main.cpp\
        src/widgets/mainwindow.cpp \
    src/widgets/viewwidget.cpp \
    src/modelloader.cpp \
    src/mesh.cpp \
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
    src/framebuffer.cpp \
    src/widgets/titlebar.cpp

HEADERS  += src/widgets/mainwindow.h \
    src/widgets/viewwidget.h \
    src/modelloader.h \
    src/mesh.h \
    src/util/vector3f.h \
    src/util/vector2f.h \
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
    src/framebuffer.h \
    src/widgets/titlebar.h

FORMS    += src/mainwindow.ui

LIBS += -L"../3rdparty/assimp/lib" -lassimp
