 
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


Pen::Pen() 
        : lineWidth_( 1. ),
          capStyle_( Qt::FlatCap ),
          joinStyle_( Qt::MiterJoin )
{
        setDashes( ResourceKey( "S", ResourceKey::BuiltIn ) );
}

Pen::Pen( const Pen& p ) 
        : lineWidth_( p.width() ),
          capStyle_( p.capStyle() ),
          joinStyle_( p.joinStyle() )
{
        setDashes( p.dashesKey_ );
}

bool Pen::setDashes( const ResourceKey& key )
{
        DashesLib& dl = DashesLib::instance();
        dashesKey_ = key;
        if ( key.isValid() )
                dashes_ = dl[dashesKey_];
        
        return true;
}

void Pen::strikePath( const QPainterPath& path, const Stroke& stroke, QPainter* painter ) const
{
        if ( !dashesKey_.isValid() )
                return;

        if ( dashes_.empty() ) {
                QPen pen( stroke.brush( path ), lineWidth_, Qt::SolidLine, capStyle_, joinStyle_ );
                painter->strokePath( path, pen );
        }
        else {
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
        stroker.setDashPattern( dashes_ );
        strokePath = stroker.createStroke( path );
}


int qHash( const Dashes& dashes )
{
        QByteArray data;
        QDataStream ds( &data, QIODevice::WriteOnly );
        
        ds << dashes;

        return qHash( data );
}
