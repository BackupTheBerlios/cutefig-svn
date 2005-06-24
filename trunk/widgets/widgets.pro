# $Id$

TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

inlcude(../cutefig_base.pri)

HEADERS = colorbutton.h \
          flagbuttongroup.h \
          errorreporter.h

SOURCES = colorbutton.cc \
          flagbuttongroup.cc \
          errorreporter.cc 
