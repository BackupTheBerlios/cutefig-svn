 
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

#include "pen.h"
#include "stroke.h"
#include "reslib.h"

#include <QPainterPath>
#include <QPainter>

#include <QDebug>

const int NoPen = -2;
const int SolidLine = -1;


template<>
ResourceKey ResLib<Dashes>::defaultKey()
{
        return ResourceKey::builtIn("S");
}

template<>
void ResLib<Dashes>::init()
{
        typedef QVector<double> dsh;
        insertBuiltIn( "S",           dsh() );
        insertBuiltIn( "Dot",         dsh() << 1 << 3 );
        insertBuiltIn( "Dash",        dsh() << 3 << 3 );
        insertBuiltIn( "DashDot",     dsh() << 3 << 3 << 1 << 3 );
        insertBuiltIn( "DashDotDot",  dsh() << 3 << 3 << 1 << 3 << 1 << 3 );
        insertBuiltIn( "DashDashDot", dsh() << 3 << 3 << 3 << 3 << 1 << 3 );
//         insertBuiltIn( "DashDotDotDot",
//                        TResourceIO<Dashes>::parseResource( QString(), "3 3 1 3 1 3 1 3") );
//         insertBuiltIn( "DashDashDashDot",
//                        TResourceIO<Dashes>::parseResource( QString(), "3 3 3 3 3 3 1 3") );
//         insertBuiltIn( "DashDashDotDot",
//                        TResourceIO<Dashes>::parseResource( QString(), "3 3 3 3 1 3 1 3") );
}



Pen::Pen() 
        : lineWidth_( 1. ),
          capStyle_( Qt::FlatCap ),
          joinStyle_( Qt::MiterJoin ),
          dashes_( new ResourceUser<Dashes>() )
{
        setDashes( ResourceKey::builtIn("S") );
}

Pen::Pen( const Pen& p ) 
        : lineWidth_( p.width() ),
          capStyle_( p.capStyle() ),
          joinStyle_( p.joinStyle() ),
          dashes_( new ResourceUser<Dashes>( *p.dashes_ ) )
{
}

Pen& Pen::operator=( const Pen& p )
{
        if ( this == &p )
                return *this;
        
        lineWidth_ = p.lineWidth_;
        capStyle_ = p.capStyle_;
        joinStyle_ = p.joinStyle_;

        delete dashes_;
        dashes_ = new ResourceUser<Dashes>( *p.dashes_ );

        return *this;
}


Pen::~Pen()
{
        delete dashes_;
}

void Pen::setDashes( const ResourceKey& key )
{
        dashes_->setResource( key );
}

void Pen::strikePath( const QPainterPath& path, const Stroke& stroke, QPainter* painter ) const
{       
        if ( !dashes_->key().isValid() )
                return;

        if ( dashes_->data().isEmpty() ) {
                QPen pen( stroke.brush( path ), lineWidth_, Qt::SolidLine, capStyle_, joinStyle_ );
                painter->strokePath( path, pen );
        } else {
                QPainterPath strokePath;
                setupPainterPath( strokePath, path );
                stroke.fillPath( strokePath, painter );
        }
}

QRectF Pen::pathRect( const QPainterPath& path ) const 
{
        QPainterPath strokePath;
        setupPainterPath( strokePath, path );

        return strokePath.boundingRect();
}

void Pen::setupPainterPath( QPainterPath& strokePath, const QPainterPath& path ) const
{
        QPainterPathStroker stroker;
        stroker.setWidth( lineWidth_ );
        stroker.setCapStyle( capStyle_ );
        stroker.setJoinStyle( joinStyle_ );
        stroker.setDashPattern( dashes_->data() );
        strokePath = stroker.createStroke( path );
}


int qHash( const Dashes& dashes )
{
        QByteArray data;
        QDataStream ds( &data, QIODevice::WriteOnly );
        
        ds << dashes;

        return qHash( data );
}

