# $Id$

#message( this is $${_FILE_} )

include( .builddir.pri )

equals(TEMPLATE,app):DESTDIR = $$BUILDDIR/bin

PRE_TARGETDEPS += $$BUILDDIR/kernel/libcutefig-core.a 

EXTRADIRS = kernel widgets

for( d, EXTRADIRS ) {
        INCLUDEPATH += $$BUILDDIR/$${d}
        DEPENDPATH += $$BUILDDIR/$${d}
}


LIBS += -L$$BUILDDIR/kernel/ -lcutefig-core \
