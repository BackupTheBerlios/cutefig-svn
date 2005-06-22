# @version $Id$

TEMPLATE = app

CUTEFIGROOT = ../..
EXPORTROOT = ..

include($$CUTEFIGROOT/cutefig_base.pri)
include($$EXPORTROOT/export_base.pri)

TARGET = cfig2pixel

HEADERS += pixoutput.h pixfilterapp.h
SOURCES += pixoutput.cc pixfilterapp.cc cfig2pixel.cc

