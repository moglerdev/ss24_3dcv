# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = Framework
QT += core opengl widgets gui openglwidgets
CONFIG += debug
DEFINES += QT_DLL QT_OPENGL_LIB QT_WIDGETS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Debug \
    ./external/eigen-3.3.9
#LIBS += -lOpengl32
CONFIG += c++20
RESOURCES +=
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/debug
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

HEADERS += ./glwidget.h \
    *.h \
    ./mainwindow.h \
    ./PointCloud.h \
    ./GLConvenience.h \
    ./QtConvenience.h \
    Axes.h \
    Cube.h \
    Hexahedron.h \
    PerspectiveCamera.h \
    Plane.h \
    Renderer.h \
    Scene.h \
    SceneObject.h \
    StereoVision.h \
    kdnode.h \
    kdtree.h \
    octnode.h \
    octtree.h

SOURCES += ./glwidget.cpp \
     ./mainwindow.cpp \
    ./main.cpp \
    ./PointCloud.cpp \
    ./GLConvenience.cpp \
    ./QtConvenience.cpp \
    Axes.cpp \
    Cube.cpp \
    Hexahedron.cpp \
    PerspectiveCamera.cpp \
    Plane.cpp \
    Renderer.cpp \
    Scene.cpp \
    SceneObject.cpp \
    StereoVision.cpp \
    kdnode.cpp \
    kdtree.cpp \
    octnode.cpp \
    octtree.cpp

FORMS += ./mainwindow.ui
