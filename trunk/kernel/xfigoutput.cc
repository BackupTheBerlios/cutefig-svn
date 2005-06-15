 
/*****************************************************************************
**
**  @version $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2005 Johannes Mueller, joh@users.berlios.net
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


#include "xfigoutput.h"

//Added by the Qt porting tool:
#include <QTextStream>

#include "allobjects.h"

#include <cmath>

XFigOutput::XFigOutput( QTextStream& ts, Figure& f )
        : OutputBackend( ts, f )
{

}

void XFigOutput::outputEllipse( Ellipse* el )
{
        drawObject_ = el;
        
        bool rad = el->isSpecifiedByRadii();
        bool circ = el->isCircle();
        int subtype = 1 +  rad + 2*circ;

        QPointF c = el->center();

        QPointF s = el->points().at(0);
        QPointF e = el->points().at(1);
        qreal rx = s.x() - e.x();
        qreal ry = s.y() - e.y();
        if ( rad ) { 
                rx *=2; ry *=2; 
        }
        
        if ( circ ) 
                rx = ry = (int) rint( sqrt( rx*rx + ry*ry ) );

        *fileStream_ << "1 " << subtype << ' ';
        outputGenericData();
        *fileStream_ << " 1 " << M_PI*el->angle()/180 << " "
                     << c.x() << ' ' << c.y() << ' ' << rx << ' ' << ry << ' '
                     << s.x() << ' ' << s.y() << ' ' << e.x() << ' ' << e.y()
                     << "\n";
}

void XFigOutput::outputPolyline( Polyline* )
{
    
}

void XFigOutput::outputPolygon( Polygon* )
{
    
}

void XFigOutput::outputHeader()
{
        *fileStream_ << "#FIG 3.2\n"
                     << "Landscape\nCenter\nMetric\nA4\n100.0\nSingle\n-2\n"
                     << "# Created by CuteFig\n"
                     << "1200 2\n";
}

void XFigOutput::outputTrailer()
{
    
}

void XFigOutput::outputGenericData()
{

        // FIXME
//         QPen p = drawObject_->pen();
        
//         *fileStream_ << (int)p.style()-1 << ' ' << p.width() << ' ' 
//                      << "-1" << ' ' << "-1" << ' '
//                      << drawObject_->depth() << ' '
//                      << "-1" << ' ' << "-1" << ' ' << "0.000";
}
