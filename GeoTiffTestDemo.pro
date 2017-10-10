#-------------------------------------------------
#
# Project created by QtCreator 2017-10-09T12:11:50
#
#-------------------------------------------------

QT       += core gui positioning location

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GeoTiffTestDemo
TEMPLATE = app

CONFIG += C++11

DEFINES += APPLICATION_NAME='"\\"$${TARGET}\\""'

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#include(libgeotiff-1.4.2/libgeotiff-1.4.2.pri)

OSGeo4W64 = C:/OSGeo4W64
INCLUDEPATH += $${OSGeo4W64}/include
DEPENDPATH += $${OSGeo4W64}/lib

LIBS += -L$${OSGeo4W64}/bin -L$${OSGeo4W64}/lib $${OSGeo4W64}/lib/tiff.lib -lgeotiff

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    QGtifReader.cpp \
    MapView.cpp \
    MapItem.cpp \
    PointItem.cpp

HEADERS += \
        mainwindow.h \
    QGtifReader.h \
    global.h \
    logging.h \
    MapView.h \
    MapItem.h \
    PointItem.h

FORMS += \
        mainwindow.ui
