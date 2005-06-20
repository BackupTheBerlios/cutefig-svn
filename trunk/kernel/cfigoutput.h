 
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

#ifndef cfigoutput_h
#define cfigoutput_h

#include "reslib.h"
#include "outputbackend.h"

#include <QTextStream>
#include <QObject>
#include <QHash>

class QPolygonF;

class CfigOutput : public OutputBackend
{
public:
        CfigOutput( QTextStream& ts, const Figure& f ) : OutputBackend( ts, f ) {}
        ~CfigOutput() {}

        virtual void outputEllipse( Ellipse* el );
        virtual void outputPolyline( Polyline* pl );
        virtual void outputPolygon( Polygon* pg );
        
        virtual void processOutput();

private:
        void outputGenericData( QString s );
        void outputPoints();
        void outputDashes();

        static const char* objectString;

        DrawObject* drawObject_;

        QHash<int,int> dashMap_;

//        ResourceTable dashTable_;
        
};

#endif
