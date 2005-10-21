# $Id$


TEMPLATE = lib
CONFIG += staticlib
TARGET = cutefig-widgets

include(../cutefig_base.pri)

#DESTDIR = .

RESOURCES += widgets.qrc

HEADERS = colorbutton.h \
          gradientbutton.h \
          strokebutton.h \
          strokeiconengines.h \
          mouseeventhandler.h \
          gradientwidget.h \
          strokedemo.h \
          flagbuttongroup.h \
          stylecombobox.h \
          strokewidget.h \
          penwidget.h \
          errorreporter.h \
          percentvalidatior.h \
          zoomcombobox.h \
          actionpushbutton.h \
          editdialog.h \
          colordialog.h \
          gradientdialog.h \
          strokemodel.h \
          strokedialog.h \ 


SOURCES = colorbutton.cc \
          gradientbutton.cc \
          strokebutton.cc \
          strokeiconengines.cc \
          mouseeventhandler.cc \
          gradientwidget.cc \
          strokedemo.cc \
          flagbuttongroup.cc \
          stylecombobox.cc \
          strokewidget.cc \
          penwidget.cc \
          errorreporter.cc \
          percentvalidatior.cc \
          zoomcombobox.cc \
          actionpushbutton.cc \
          editdialog.cc \
          colordialog.cc \
          gradientdialog.cc \
          strokemodel.cc \
          strokedialog.cc 
