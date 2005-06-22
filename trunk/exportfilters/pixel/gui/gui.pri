# $Id$

isEmpty(EXPORTROOT):error(You must set EXPORTROOT before include()ing $${_FILE_})

PIXELROOT = $$EXPORTROOT/pixel

INCLUDEPATH += $$PIXELROOT $$PIXELROOT/gui

HEADERS += $$PIXELROOT/gui/pixoutdialog.h $$PIXELROOT/pixoutput.h
SOURCES += $$PIXELROOT/gui/pixoutdialog.cc $$PIXELROOT/pixoutput.cc
