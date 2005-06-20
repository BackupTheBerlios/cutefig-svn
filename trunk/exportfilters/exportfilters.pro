# @version $Id$

TEMPLATE = lib
TARGET = exportfilters
CONFIG += staticlib
CONFIG += debug

PRE_TARGETDEPS += ../kernel/libcutefig-core.a
INCLUDEPATH += ../kernel 
DEPENDPATH += ../kernel
OBJECTS_DIR += pixel

LIBS += -L ../kernel -lcutefig-core

HEADERS += exportfilter.h filterlib.h
SOURCES += filterlib.cc
