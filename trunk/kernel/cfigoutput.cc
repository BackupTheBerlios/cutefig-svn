 
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
#include "gradient.h"

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

QTextStream& operator<< ( QTextStream& ts, const ResourceKey& key )
{
        if ( !key.isValid() ) {
                ts << '%';
                return ts;
        }
        
        if ( key.isBuiltIn() )
                ts << '&';
        else
                ts << '*';

        ts << key.keyString();

        return ts;
}

QTextStream& operator<< ( QTextStream& ts, const Stroke& st )
{
        if ( st.isHardColor() )
                ts << st.color();
        else
                ts << st.key();

        return ts;
}

void CfigOutput::outputGenericData( QString name )
{
        const Pen& p = drawObject_->pen();

        if ( !drawObject_->comment().isEmpty() ) {
                QStringList comments = drawObject_->comment().split('\n');
                foreach ( QString s, comments ) 
                        fileStream_ << "# " << s << '\n';
        }
        
        fileStream_ << "object " << name << ' ' << drawObject_->points().size() << ' ';
        
        fileStream_ << p.width() << ' ' << p.dashesKey() << ' '
                    << (int)p.capStyle() << ' ' << (int)p.joinStyle() << ' '
                    << drawObject_->stroke() << ' ' << drawObject_->fill() << ' '
                    << drawObject_->depth();
}

void CfigOutput::outputPoints()
{
        foreach ( QPointF p, drawObject_->points() )
                fileStream_ << "point " << p.x() << ' ' << p.y() << "\n";
}

void CfigOutput::outputDashes()
{
        DashesLib& dl = DashesLib::instance();

        foreach ( ResourceKey key, figure_.dashList() ) {
                if ( !key.isBuiltIn() ) {
                        fileStream_ << "dashes " << key.keyString();
                        foreach ( double d, dl[key] )
                                fileStream_ << ' ' << d;
                        fileStream_ << "\n";
                }
        }
}

QTextStream& operator<< ( QTextStream& ts, Gradient* grad ) 
{
        QGradientStops& stops = grad->colorStops();

        LinearGradient* lg = 0;
        RadialGradient* rg = 0;
        
        if ( grad->type() == Gradient::Linear ) {
                lg = static_cast<LinearGradient*>( grad );
                ts << "linear ";
        }
        else {
                rg = static_cast<RadialGradient*>( grad );
                ts << "radial ";
        }

        ts << stops.first().second << ' ' << stops.last().second << ' ';

        if ( lg )
                ts << lg->startPoint().x() << ' ' << lg->startPoint().y()  << ' '
                   << lg->finalPoint().x() << ' ' << lg->finalPoint().y();
        else
                ts << rg->centerPoint().x() << ' ' << rg->centerPoint().y() << ' '
                   << rg->focalPoint().x() << ' ' << rg->focalPoint().y() << ' '
                   << rg->radius();

        ts << "\n";

        for ( int i = 1; i < stops.size()-1; i++ )
                ts << "gradstop " << stops[i].first << ' ' << stops[i].second << "\n";

        ts << "gradend\n";

        return ts;
}

void CfigOutput::outputStrokes()
{
        StrokeLib& sl = StrokeLib::instance();

        foreach ( ResourceKey key, figure_.strokeList() ) {
                Stroke stroke = sl[key];
                switch ( strokeType( stroke ) ) {
                    case Stroke::sColor: 
                    {
                            const QColor& c = strokeColor( stroke );
                            fileStream_ << "color " << key.keyString() << ' ' << c << "\n";
                            break;
                    }
                    case Stroke::sGradient: 
                    {
                            Gradient* grad = strokeGradient( stroke );
                            fileStream_ << "gradient " << key.keyString() << ' ' << grad;
                            break;
                    }
                    default:
                            break;
                }
        }
}
