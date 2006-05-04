 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.de
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License version 2
**  as published by the Free Software Foundation.
** 
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
******************************************************************************/


/** \mainpage The CuteFig source documentation
 *  \version 0.1
 *  \author Johannes Mueller <joh@users.berlios.de>
 *
 *  \section intro_sec Introduction
 *
 *  CuteFig is written in C++ using the QT-Library of TrollTech AS
 *  (http://www.trolltech.com/) to do the GUI-stuff, the rendering and
 *  the necessary abstractions to be platform independent.
 *
 *  The source is split up into three directories:
 *      - kernel, this is the non-gui stuff
 *      - gui_app, the userfrontend
 *
 *  In the kernel-directory there is stuff like the classes for the
 *  drawing objects, the IO-stuff (parsers, import/export filters)
 * 
 *
 *  \section desing_sec Design issues
 *
 *  \ref resource_system
 *  
 *  \ref action_system
 *
 *  \ref mouse_interaction
 *  
 *  \subsection mvc_sec Model-View-Controler
 *
 *  Like many modern GUI-systems CuteFig uses a Model-View-Controler
 *  (MVC) approach. The View is the CanvasView class.
 */

#include "cutefig.h"
#include <QApplication>

int main( int argc, char* argv[] )
{        
        QApplication a( argc, argv );

        QCoreApplication::setApplicationName("CuteFig");
        QCoreApplication::setOrganizationName("BerliOS");
        QCoreApplication::setOrganizationDomain("berlios.de");

        Q_INIT_RESOURCE( widgets );
        (new CuteFig())->show();
        a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
        return a.exec();
}
