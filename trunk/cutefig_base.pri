# $Id$

#message( this is $${_FILE_} )


BUILDDIR = /home/joh/devel/cutefig

equals(TEMPLATE,app):DESTDIR = $$BUILDDIR/bin

PRE_TARGETDEPS += $$BUILDDIR/kernel/libcutefig-core.a 

INCLUDEPATH += $$BUILDDIR/kernel 
DEPENDPATH += $$BUILDDIR/kernel 

LIBS += -L$$BUILDDIR/kernel/ -lcutefig-core
