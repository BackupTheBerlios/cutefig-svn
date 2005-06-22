# This is the main project file of the CuteFig:
#
# @version $Id$
#

system( echo BUILDDIR=$PWD > .builddir.pri )

CONFIG+=debug

TEMPLATE = subdirs

SUBDIRS = kernel exportfilters gui_app


DEFINES += QT_COMPAT_WARNINGS
