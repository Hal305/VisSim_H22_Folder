QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++11
INCLUDEPATH += .\Matrix4x4\
TARGET      = Rulleball

FORMS += \
    mainwindow.ui

HEADERS += \
    barycentriccalc.h \
    bsplinecurve.h \
    camera.h \
    input.h \
    light \
    logger.h \
    mainwindow.h \
    octaball.h \
    quadtree.h \
    quicksort.h \
    raindrop.h \
    renderwindow.h \
    rollingball.h \
    shader.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h \
    xyz.h

SOURCES += \
    barycentriccalc.cpp \
    bsplinecurve.cpp \
    camera.cpp \
    light.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    octaball.cpp \
    quicksort.cpp \
    raindrop.cpp \
    renderwindow.cpp \
    rollingball.cpp \
    shader.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

DISTFILES += \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert
