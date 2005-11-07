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
           drawobjectio.h \
           pen.h \
           stroke.h \
           gradient.h \
           resourcekey.h \
           reslib.h \
           strokelib.h \
           initialiser.h \
           reslibinit.h \
           streamops.h \
           stdresio.h \
           resourceio.h \
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
           drawobjectio.cc \
           pen.cc \
           stroke.cc \
           gradient.cc \
           strokelib.cc \
           initialiser.cc \
           reslibinit.cc \
           resourceio.cc \
           stdresio.cc \
           streamops.cc \
           parser.cc \
           cfigoutput.cc \
#           xfigoutput.cc \
           progutils.cc

