# @version $Id$

QT -= qt3support

TEMPLATE = lib
TARGET = cutefig-core
CONFIG += staticlib

# Input
HEADERS += fig.h \
           typedefs.h \
           geometry.h \
           figure.h \
           drawobject.h \
           compound.h \
           ellipse.h \
           polyline.h \
           polygon.h \
           pen.h \
           stroke.h \
           brush.h \
           brushdata.h \
           reslib.h \
           reslibinit.h \
           parser.h \
           cfigoutput.h \
           outputbackend.h \
#           xfigoutput.h \
           progutils.h 
SOURCES += figure.cc \
           geometry.cc \
           drawobject.cc \
           compound.cc \
           ellipse.cc \
           polyline.cc \
           polygon.cc \
           pen.cc \
           stroke.cc \
           brush.cc \
           reslibinit.cc \
           parser.cc \
           cfigoutput.cc \
#           xfigoutput.cc \
           progutils.cc

