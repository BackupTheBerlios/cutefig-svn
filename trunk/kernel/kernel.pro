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
           gradient.h \
           resourcekey.h \
           reslib.h \
           strokelib.h \
           reslibinit.h \
           resourceio.h \
           streamops.h \
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
           gradient.cc \
           strokelib.cc \
           reslibinit.cc \
           resourceio.cc \
           stdresio.cc \
           streamops.cc \
           parser.cc \
           cfigoutput.cc \
#           xfigoutput.cc \
           progutils.cc

