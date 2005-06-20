 
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

#ifndef savebackend_h
#define savebackend_h

class Figure;
class DrawObject;
class Ellipse;
class Polyline;
class Polygon;

class QTextStream;

#include <QStringList>

class SaveBackend : public OutputBackend
{
public:
        SaveBackend( QTextStream& ts, const Figure& f )
                : OutputBackend( f ),
                  fileStream_( ts ) {}
        virtual ~SaveBackend() { };

        virtual void saveEllipse( Ellipse* el ) = 0;
        virtual void savePolyline( Polyline* pl ) = 0;
        virtual void savePolygon( Polygon* pg ) = 0;

        virtual void processSave() = 0;

        QStringList& errorReport() { return errorReport_; };
        
protected:
        QStringList errorReport_;
        const Figure& figure_;
        QTextStream& fileStream_;
        
};

#endif
