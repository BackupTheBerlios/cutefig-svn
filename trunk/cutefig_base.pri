# $Id$

CONFIG += debug

BUILDDIR = $${PWD}

equals(TEMPLATE,app):DESTDIR = $$BUILDDIR/bin

win32 {
        debug {
                DEBUGRELEASE=debug
        }
        !debug {
                DEBUGRELEASE=release
        }
}      

message($$DEBUGRELEASE)

PRE_TARGETDEPS += $$BUILDDIR/kernel/$$DEBUGRELEASE/libcutefig-core.a 

EXTRADIRS = kernel widgets

for( d, EXTRADIRS ) {
        INCLUDEPATH += $$BUILDDIR/$${d}
        DEPENDPATH += $$BUILDDIR/$${d}
}


LIBS += -L$$BUILDDIR/kernel/$$DEBUGRELEASE -lcutefig-core \
