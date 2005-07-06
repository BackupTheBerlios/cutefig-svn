 
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

void CfigOutput::outputCompound( Compound* cd )
{
        const ObjectList& objects = cd->childObjects();
        fileStream_ << "compound_begin\n";
        foreach ( DrawObject* o, objects ) 
                o->outputToBackend( this );
        fileStream_ << "compound_end\n";
}

void CfigOutput::processOutput()
{
        outputDashes();
        outputStrokes();
        figure_.outputObjects( this );
}

void put_colorPart( QTextStream& ts, int v )
{
        int e = v & 0x0F;
        e += (e>9) ? 'A'-10 : '0';
        v = v >> 4;
        v += (v>9) ? 'A'-10 : '0';
        
        ts << (char)v << (char)e;
}


QTextStream& operator<< ( QTextStream& ts, const QColor& c )
{
        ts << '#';
        
        put_colorPart( ts, c.red() );
        put_colorPart( ts, c.green() );
        put_colorPart( ts, c.blue() );
        
        if ( c.alpha() != 0xFF )
                put_colorPart( ts, c.alpha() );
        
        return ts;
}

void CfigOutput::outputGenericData( QString name )
{
        const Pen& p = drawObject_->pen();
        int dk = p.dashesKey();
        if ( dk >= 0 )
                dk = dashMap_[dk];

        if ( !drawObject_->comment().isEmpty() ) {
                QStringList comments = drawObject_->comment().split('\n');
                foreach ( QString s, comments ) 
                        fileStream_ << "# " << s << '\n';
        }
        
        fileStream_ << "object " << name << ' ' << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << dk << ' '
                    << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                    << p.color() << ' ';

        const Stroke& fs = drawObject_->fillStroke();
        if ( fs ) 
                if ( fs.key().isNull() )
                        fileStream_ << strokeColor( fs );
                else
                        fileStream_ << '*' << fs.key();
        else
                fileStream_ << '%';

        fileStream_ << ' ' << drawObject_->depth();
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

void CfigOutput::outputStrokes()
{
        StrokeLib& sl = StrokeLib::instance();

        foreach ( QString key, figure_.strokeList() ) {
                Stroke stroke = sl[key];
                switch ( strokeType( stroke ) ) {
                    case Stroke::Color: 
                    {
                            const QColor& c = strokeColor( stroke );
                            fileStream_ << "color " << key << ' ' << c << ' '
                                        << "\n";
                            break;
                    }
                    default:
                            break;
                }
        }                   
}
