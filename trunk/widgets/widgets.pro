# $Id$


TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

include(../cutefig_base.pri)

#DESTDIR = .

RESOURCES += widgets.qrc

HEADERS = paintutils.h \
          resourcebutton.h \
          colorbutton.h \
          fontbutton.h \
          resourceiconengine.h \
          mouseeventhandler.h \
          gradientwidget.h \
          resourcedemo.h \
          flagbuttongroup.h \
          stylecombobox.h \
          strokewidget.h \
          penwidget.h \
          errorreporter.h \
          percentvalidator.h \
          zoomcombobox.h \
          actionpushbutton.h \
          editdialog.h \
          colordialog.h \
          resourcedialog.h \
          gradienteditor.h \
          resourcemodel.h \
          reslibeditor.h


SOURCES = paintutils.cc \
          fontbutton.cc \
          resourceiconengine.cc \
          mouseeventhandler.cc \
          gradientwidget.cc \
          resourcedemo.cc \
          flagbuttongroup.cc \
          stylecombobox.cc \
          strokewidget.cc \
          penwidget.cc \
          errorreporter.cc \
          percentvalidator.cc \
          zoomcombobox.cc \
          actionpushbutton.cc \
          editdialog.cc \
          colordialog.cc \
          gradienteditor.cc \
          reslibeditor.cc
