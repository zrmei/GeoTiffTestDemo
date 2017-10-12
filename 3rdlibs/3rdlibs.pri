INCLUDEPATH += $$PWD/geotiff $$PWD/proj-4.9.3
DEPENDPATH += $$PWD

LIBS += -L$$PWD $$PWD/xtiff.lib -lgeotiff -ltiff -lproj
