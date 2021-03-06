# $Id$


TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

include(../cutefig_base.pri)

#DESTDIR = .

RESOURCES += widgets.qrc

HEADERS = paintutils.h \
          layouter.h \
          resourcebutton.h \
          colorbutton.h \
          chainbutton.h \
          fontbutton.h \
          fontwidget.h \
          resourceiconengine.h \
          mouseeventhandler.h \
          gradientwidget.h \
          resourcedemo.h \
          flagbuttongroup.h \
          resourcecombobox.h \
          strokewidget.h \
          penwidget.h \
          arrowwidget.h \
          errorreporter.h \
          percentvalidator.h \
          actionpushbutton.h \
          editdialog.h \
          colordialog.h \
          resourcedialog.h \
          gradienteditor.h \
          resourcemodel.h \
          reslibeditor.h


SOURCES = paintutils.cc \
          layouter.cc \
          chainbutton.cc \
          fontbutton.cc \
          fontwidget.cc \
          resourceiconengine.cc \
          mouseeventhandler.cc \
          gradientwidget.cc \
          resourcedemo.cc \
          flagbuttongroup.cc \
          resourcecombobox.cc \
          strokewidget.cc \
          penwidget.cc \
          arrowwidget.cc \
          errorreporter.cc \
          percentvalidator.cc \
          actionpushbutton.cc \
          editdialog.cc \
          colordialog.cc \
          gradienteditor.cc \
          reslibeditor.cc
