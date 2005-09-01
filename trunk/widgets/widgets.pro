# $Id$


TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

include(../cutefig_base.pri)

#DESTDIR = .

RESOURCES += widgets.qrc

HEADERS = colorbutton.h \
          gradientbutton.h \
          strokeiconengines.h \
          mouseeventhandler.h \
          gradientwidget.h \
          flagbuttongroup.h \
          stylecombobox.h \
          strokewidget.h \
          penwidget.h \
          errorreporter.h \
          percentvalidatior.h \
          zoomcombobox.h \
          editdialog.h \
          gradientdialog.h 


SOURCES = colorbutton.cc \
          gradientbutton.cc \
          strokeiconengines.cc \
          mouseeventhandler.cc \
          gradientwidget.cc \
          flagbuttongroup.cc \
          stylecombobox.cc \
          strokewidget.cc \
          penwidget.cc \
          errorreporter.cc \
          percentvalidatior.cc \
          zoomcombobox.cc \
          editdialog.cc \
          gradientdialog.cc
