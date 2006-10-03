# @version $Id$

TEMPLATE = lib

EXPORTROOT = ..
CUTEFIGROOT = ../..

include($$CUTEFIGROOT/cutefig_base.pri)
include($$EXPORTROOT/export_base.pri)

DESTDIR = .

TARGET = exportfilters
CONFIG += staticlib

SUBDIRS = pixel

HEADERS += filterlib.h exportgui.h
SOURCES += filterlib.cc exportgui.cc

FILTERS += pixel

for( f, FILTERS ):include($$EXPORTROOT/$${f}/gui/gui.pri)
