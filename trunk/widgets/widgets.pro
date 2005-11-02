# $Id$


TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

include(../cutefig_base.pri)

#DESTDIR = .

RESOURCES += widgets.qrc

HEADERS = resourcebutton.h \
          colorbutton.h \
          resourceiconengine.h \
          mouseeventhandler.h \
          gradientwidget.h \
          strokedemo.h \
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


SOURCES = resourceiconengine.cc \
          mouseeventhandler.cc \
          gradientwidget.cc \
          strokedemo.cc \
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
