# @version $Id$

TARGET = cutefig
CONFIG += thread 
TEMPLATE = app

#QT+=qt3support

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
           controler.h \
           clipboard.h \
           selection.h \
           actioncollection.h \
           actions.h \
           allactions.h\
           allodialogs.h\
           objectguihandler.h \
           interactiveaction.h \
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
           command.h \
           objectcommand.h \
           addcommand.h \
           deletecommand.h \
           changecommand.h \
#           colortableitem.h \
           lineshowwidget.h \
           ruler.h \
           griddialog.h \
           objectdialog.h \
           ellipsedialog.h \
           textobjectdialog.h \
           polylinedialog.h \
           polygondialog.h 
SOURCES += cutefig.cc \
           guiinit.cc \
           centralwidget.cc \
           canvasview.cc \
           controler.cc \
           selection.cc \
           actioncollection.cc \
           actions.cc \
           objectguihandler.cc \
           interactiveaction.cc \
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
           command.cc \
           addcommand.cc \
           deletecommand.cc \
           changecommand.cc \
#           colortableitem.cc \
           lineshowwidget.cc \
           ruler.cc \
           griddialog.cc \
           objectdialog.cc \
           ellipsedialog.cc \
           textobjectdialog.cc \
           polylinedialog.cc \
           polygondialog.cc \
           main.cc

TRANSLATIONS = cutefig_de.ts

