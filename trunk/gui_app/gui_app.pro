# @version $Id$

TARGET = cutefig
CONFIG += thread 
CONFIG += debug
TEMPLATE = app

#QT+=qt3support

include(../cutefig_base.pri)

# Widgets
LIBS += -L$$BUILDDIR/widgets/$$DEBUGRELEASE -lcutefig-widgets
PRE_TARGETDEPS += $$BUILDDIR/widgets/$$DEBUGRELEASE/libcutefig-widgets.a

# Exportfilters
INCLUDEPATH += $$BUILDDIRexportfilters/ $$BUILDDIR/exportfilters/gui_lib
LIBS += -L$$BUILDDIR/exportfilters/$$DEBUGRELEASE/gui_lib -lexportfilters
PRE_TARGETDEPS += $$BUILDDIR/exportfilters/gui_lib/$$DEBUGRELEASE/libexportfilters.a

RESOURCES = cutefig.qrc

# Input
HEADERS += cutefig.h \
           centralwidget.h \
           viewbase.h \
           canvasview.h \
           controler.h \
           clipboard.h \
           selection.h \
           actions.h \
           allactions.h\
           allodialogs.h\
           objectmapper.h \
           objecthandler.h\
           interactiveaction.h \
           relativeaction.h \
           scaleaction.h \
           createaction.h \
           pointmoveaction.h \
           deleteaction.h \
           cutnpaste.h \
           groupungroup.h \
           editdialogaction.h \
           command.h \
           addcommand.h \
           deletecommand.h \
           changecommand.h \
#           colortableitem.h \
           stylecombobox.h \
           lineshowwidget.h \
           ruler.h \
           objectdialog.h \
           ellipsedialog.h \
           polylinedialog.h \
           polygondialog.h 
SOURCES += cutefig.cc \
           centralwidget.cc \
           canvasview.cc \
           controler.cc \
           selection.cc \
           actions.cc \
           objectmapper.cc \
           objecthandler.cc \
           interactiveaction.cc \
           relativeaction.cc \
           scaleaction.cc \
           createaction.cc \
           pointmoveaction.cc \
           deleteaction.cc \
           cutnpaste.cc \
           groupungroup.cc \
           editdialogaction.cc \
           command.cc \
           addcommand.cc \
           deletecommand.cc \
           changecommand.cc \
#           colortableitem.cc \
           stylecombobox.cc \
           lineshowwidget.cc \
           ruler.cc \
           objectdialog.cc \
           ellipsedialog.cc \
           polylinedialog.cc \
           polygondialog.cc \
           main.cc

TRANSLATIONS = cutefig_de.ts

