# This is the main project file of the CuteFig:
#
# @version $Id$
#

CONFIG+=debug

TEMPLATE = subdirs

SUBDIRS = kernel gui_app

QT += qt3Support
DEFINES += QT_COMPAT_WARNINGS
