# @version $Id: exportfilters.pro 8 2005-06-21 06:30:27Z joh $

TEMPLATE = lib

EXPORTROOT = ..
CUTEFIGROOT = ../..

include($$CUTEFIGROOT/cutefig_base.pri)
include($$EXPORTROOT/export_base.pri)

DESTDIR = .

TARGET = exportfilters
CONFIG += staticlib

SUBDIRS = pixel

HEADERS += filterlib.h exportdialog.h exportgui.h
SOURCES += filterlib.cc exportdialog.cc exportgui.cc

FILTERS += pixel

for( f, FILTERS ):include($$EXPORTROOT/$${f}/gui/gui.pri)
