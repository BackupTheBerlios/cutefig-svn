# @version $Id$

defineReplace(sources) {
        names = $$ARGS
        sources = 
        for( name, names ) {
                sources += $${name}.cc
        }
        return ($$sources)
}

defineReplace(objects) {
        names = $$ARGS
        objects = 
        for( name, names ) {
                objects += $${name}.o
        }
        return ($$objects)
}

TEMPLATE=app
TARGET=cfig2pixel

PRE_TARGETDEPS += ../../kernel/libcutefig-core.a
INCLUDEPATH += ../ ../../kernel 
DEPENDPATH += ../ ../../kernel

LIBS += -L ../../kernel -lcutefig-core

HEADERS += pixoutput.h pixfilterapp.h
GENERIC_SOURCEFILES = pixoutput pixfilterapp

CFIG2PNG_SOURCEFILES = cfig2png

SOURCES +=  $$sources($$GENERIC_SOURCEFILES) main.cc
GENERIC_OBJECTS = $$objects($$GENERIC_SOURCEFILES)

cfig2png.target = cfig2png
cfig2png.objects = $$GENERIC_OBJECTS $$objects($$CFIG2PNG_SOURCEFILES)
cfig2png.commands = $$QMAKE_LINK $$QMAKE_LFLAGS -o $$cfig2png.target $$cfig2png.objects $(LIBS)
cfig2png.depends = $$cfig2png.objects $$sources($$CFIG2PNG_SOURCEFILES) 

QMAKE_EXTRA_UNIX_TARGETS += cfig2png
PRE_TARGETDEPS += cfig2png

