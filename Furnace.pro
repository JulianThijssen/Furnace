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
    #src/modelloader.cpp \
    src/vector3f.cpp \
    src/vector2f.cpp

HEADERS  += src/mainwindow.h \
    src/viewwidget.h \
    #src/modelloader.h \
    #src/mesh.h \
    src/vector3f.h \
    src/vector2f.h \
    src/face.h

FORMS    += src/mainwindow.ui

LIBS += -L"../3rdparty/assimp/lib" -lassimp
