 
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


#include "cfigoutput.h"
#include "allobjects.h"
#include "pen.h"

#include <QPolygonF>
#include <QTextStream>

const char* CfigOutput::objectString = "object ";

void CfigOutput::outputEllipse( Ellipse* el )
{
        drawObject_ = el;
        outputGenericData( "ellipse" );
        fileStream_.setRealNumberPrecision( 3 );
        fileStream_ << ' ' << el->isCircle() << ' ' 
                    << el->isSpecifiedByRadii() << ' ' << el->angle()
                    << "\n";
        outputPoints();
}

void CfigOutput::processOutput()
{
        outputDashes();
        figure_.outputObjects( this );
}


void CfigOutput::outputPolyline( Polyline* pl )
{
        drawObject_ = pl;
        outputGenericData( "polyline" );
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputPolygon( Polygon* pg )
{
        drawObject_ = pg;
        outputGenericData( "polygon" );
        fileStream_ << "\n";
        outputPoints();
}

void CfigOutput::outputGenericData( QString name )
{
        const Pen& p = drawObject_->pen();
        int dk = p.dashesKey();
        if ( dk >= 0 )
                dk = dashMap_[dk];
        
        QStringList comments = drawObject_->comment().split('\n');
        foreach ( QString s, comments ) 
                fileStream_ << "# " << s << '\n';

        fileStream_ << "object " << name << ' ' << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << dk << ' '
                     << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                     << p.color().name() << ' ' << p.color().alphaF() << ' ';

        fileStream_ << drawObject_->fillColor().name() << ' ' 
                     << drawObject_->fillColor().alphaF() << ' '
                     << -1 << ' ' << drawObject_->depth();
}

void CfigOutput::outputPoints()
{
        foreach ( QPointF p, drawObject_->points() )
                fileStream_ << "point " << p.x() << ' ' << p.y() << "\n";
}

void CfigOutput::outputDashes()
{
        DashesLib& dl = DashesLib::instance();
        dashMap_ = QHash<int,int>();

        int i = 0;
        foreach ( int key, figure_.dashList() ) {
                dashMap_[key] = i++;
                fileStream_ << "dashes";
                foreach( double d, dl[key] )
                        fileStream_ << " " << d;
                fileStream_ << "\n";
        }
}

