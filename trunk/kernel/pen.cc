 
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

#include "pen.h"
#include "reslib.h"

#include <QPainterPath>
#include <QPainter>

#include <QDebug>

DashesLib& Pen::dashesLib_ = DashesLib::instance();

const int NoPen = -6;
const int SolidLine = -5;


Pen::Pen() 
        : lineWidth_( 1. ),
          color_( Qt::black ),
          capStyle_( Qt::FlatCap ),
          joinStyle_( Qt::MiterJoin ),
          dashesKey_( SolidLine )
{
}

Pen::Pen( const Pen& p ) 
        : lineWidth_( p.width() ),
          color_( p.color() ),
          capStyle_( p.capStyle() ),
          joinStyle_( p.joinStyle() ),
          dashesKey_( p.dashesKey() ),
          dashes_( p.dashes_ )
{
}

bool Pen::setDashes( int key )
{
        if ( key < 0 ) {
                dashesKey_ = key;
                return true;
        }

        if ( key > dashesLib_.size() - 1 ) {
                dashesKey_ = NoPen;
                return false;
        }

        dashesKey_ = key;
        dashes_ = dashesLib_[dashesKey_];

        return true;
}

void Pen::drawPath( const QPainterPath& path, QPainter* painter ) const
{

        if ( dashesKey_ < 0 ) {
                QPen pen( color_, lineWidth_, Qt::PenStyle(dashesKey_+6), capStyle_, joinStyle_ );
                painter->strokePath( path, pen );
        } else {
                QPainterPathStroker stroker;
                stroker.setWidth( lineWidth_ );
                stroker.setCapStyle( capStyle_ );
                stroker.setJoinStyle( joinStyle_ );
                stroker.setDashPattern( dashes_ );
                QPainterPath strokePath = stroker.createStroke( path );
                painter->fillPath( strokePath, color_ );
        }
}
