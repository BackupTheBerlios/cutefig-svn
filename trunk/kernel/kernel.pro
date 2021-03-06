# @version $Id$

QT -= qt3support

TEMPLATE = lib
TARGET = cutefig-core
CONFIG += staticlib

# Input
HEADERS += fig.h \
           allobjects.h \
           keywords.h \
           typedefs.h \
           autohash.h \
           length.h \
           paper.h \
           geometry.h \
           figure.h \  
           resourcekey.h \
           resourceuser.h \
           reslib.h \
           initialiser.h \
           streamops.h \
           resourceio.h \
           parser.h \
           cfigoutput.h \
           drawobject.h \
           compound.h \
           rectangloid.h \
           rectangle.h \
           ellipse.h \
           polyline.h \
           polygon.h \
           bezierspline.h \
           arc.h \
           textobject.h \
           arrow.h \
           objecthandler.h \
           pen.h \
           stroke.h \
           pattern.h \
           gradient.h \
           pixmap.h \
           outputbackend.h 
SOURCES += figure.cc \
           length.cc \
           paper.cc \
           geometry.cc \
           reslib.cc \
           initialiser.cc \
           resourceio.cc \
           streamops.cc \
           parser.cc \
           cfigoutput.cc \
           pen.cc \
           stroke.cc \
           pattern.cc \
           gradient.cc \
           pixmap.cc \
           drawobject.cc \
           compound.cc \
           rectangloid.cc \
           rectangle.cc \
           ellipse.cc \
           polyline.cc \
           polygon.cc \
           bezierspline.cc \
           arc.cc \
           textobject.cc \
           arrow.cc \
           objecthandler.cc 

