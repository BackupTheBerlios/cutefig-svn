 
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

#ifndef outputbackend_h
#define outputbackend_h

class Figure;
class DrawObject;
class Ellipse;
class Polyline;
class Polygon;
class Compound;

//class QTextStream;


#include "stroke.h"
#include "gradient.h"

#include <ostream>
#include <QStringList>

class OutputBackend
{
public:
        OutputBackend( std::ostream& ts, const Figure& f ) 
                : figure_( f ),
                  fileStream_( ts ),
                  errorReport_() {}
        virtual ~OutputBackend() {}

        virtual void outputEllipse( Ellipse* el ) = 0;
        virtual void outputPolyline( Polyline* pl ) = 0;
        virtual void outputPolygon( Polygon* pg ) = 0;

        virtual void outputCompound( Compound* cd ) = 0;
        
        virtual void processOutput() = 0;

        QStringList& errorReport() { return errorReport_; }
        
protected:
        Stroke::StrokeType strokeType( const Stroke& s ) { return s.type(); }
        
        const Figure& figure_;
        std::ostream& fileStream_;
        QStringList errorReport_;        
};

#endif
