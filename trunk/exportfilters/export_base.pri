# $Id$

#message( this is $${_FILE_} )

isEmpty(EXPORTROOT):error(You must set EXPORTROOT before include()ing $${_FILE_})

INCLUDEPATH += $$EXPORTROOT

OBJECTS_DIR = $$EXPORTROOT/.obj