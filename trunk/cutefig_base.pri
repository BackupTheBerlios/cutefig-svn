# $Id$

#message( this is $${_FILE_} )

include( .builddir.pri )

equals(TEMPLATE,app):DESTDIR = $$BUILDDIR/bin

PRE_TARGETDEPS += $$BUILDDIR/kernel/libcutefig-core.a 

INCLUDEPATH += $$BUILDDIR/kernel 
DEPENDPATH += $$BUILDDIR/kernel 

LIBS += -L$$BUILDDIR/kernel/ -lcutefig-core
