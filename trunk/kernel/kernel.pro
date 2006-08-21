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
           gradient.h \
           outputbackend.h \
#           xfigoutput.h \
           progutils.h 
SOURCES += figure.cc \
           length.cc \
           paper.cc \
           geometry.cc \
           reslib.cc \
           initialiser.cc \
           resourceio.cc \
           stdresio.cc \
           streamops.cc \
           parser.cc \
           cfigoutput.cc \
           pen.cc \
           stroke.cc \
           gradient.cc \
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
           objecthandler.cc \
#           xfigoutput.cc \
           progutils.cc

