# $Id$

TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

DESTDIR = .

inlcude(../cutefig_base.pri)

HEADERS = colorbutton.h \
          flagbuttongroup.h \
          errorreporter.h

SOURCES = colorbutton.cc \
          flagbuttongroup.cc \
          errorreporter.cc 
