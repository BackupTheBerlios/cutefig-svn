# @version $Id$

TARGET = cutefig
CONFIG += thread #assistant 
TEMPLATE = app

#QT += opengl

include(../cutefig_base.pri)

# Widgets
LIBS += -L$$BUILDDIR/widgets/$$DEBUGRELEASE -lcutefig-widgets
PRE_TARGETDEPS += $$BUILDDIR/widgets/$$DEBUGRELEASE/libcutefig-widgets.a

# Exportfilters
INCLUDEPATH += $$BUILDDIRexportfilters/ $$BUILDDIR/exportfilters/gui_lib
LIBS += -L$$BUILDDIR/exportfilters/gui_lib/$$DEBUGRELEASE -lexportfilters
PRE_TARGETDEPS += $$BUILDDIR/exportfilters/gui_lib/$$DEBUGRELEASE/libexportfilters.a

RESOURCES = cutefig.qrc

# Input
HEADERS += cutefig.h \
           guiinit.h \
           centralwidget.h \
           viewbase.h \
           canvasview.h \
           statuswidgets.h \
           controler.h \
           clipboard.h \
           selection.h \
           actioncollection.h \
           actions.h \
           allactions.h\
           recentfiles.h \
           allodialogs.h\
           objectguihandler.h \
           interactiveaction.h \
           actionstatus.h \
           relativeaction.h \
           scaleaction.h \
           createaction.h \
           textaction.h \
           textpropaction.h \
           texttagaction.h \
           pointmoveaction.h \
           deleteaction.h \
           cutnpaste.h \
           groupungroup.h \
           editdialogaction.h \
           propdialogaction.h \
           command.h \
           objectcommand.h \
           addcommand.h \
           deletecommand.h \
           changecommand.h \
           ruler.h \
           propdialog.h \
           griddialog.h \
           objectdialog.h \
           rectangloiddialog.h \
           textobjectdialog.h \
           pointflagscalc.h

SOURCES += cutefig.cc \
           guiinit.cc \
           centralwidget.cc \
           canvasview.cc \
           statuswidgets.cc \
           controler.cc \
           selection.cc \
           actioncollection.cc \
           actions.cc \
           recentfiles.cc \
           objectguihandler.cc \
           interactiveaction.cc \
           actionstatus.cc \
           relativeaction.cc \
           scaleaction.cc \
           createaction.cc \
           textaction.cc \
           textpropaction.cc \
           texttagaction.cc \
           pointmoveaction.cc \
           deleteaction.cc \
           cutnpaste.cc \
           groupungroup.cc \
           editdialogaction.cc \
           propdialogaction.cc \
           command.cc \
           objectcommand.cc \
           addcommand.cc \
           deletecommand.cc \
           changecommand.cc \
           ruler.cc \
           propdialog.cc \
           griddialog.cc \
           objectdialog.cc \
           rectangloiddialog.cc \
           textobjectdialog.cc \
           pointflagscalc.cc \
           main.cc

TRANSLATIONS = cutefig_de.ts

