 
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


#include "gradient.h"

#include <QDebug>

void Gradient::ensureRange( qreal& val )
{
        if( val > 1. )
                val = 1.;
        else if ( val < 0 )
                val = 0;
}

void Gradient::ensureRange( QPointF& point )
{
        ensureRange( point.rx() );
        ensureRange( point.ry() );
}

void LinearGradient::setStartPoint( const QPointF& p )
{
        start_ = p;
        ensureRange( start_ );
}

void LinearGradient::setFinalPoint( const QPointF& p )
{
        final_ = p;
        ensureRange( final_ );
}

void Gradient::setColorAt( qreal pos, const QColor& c )
{
        colorStops_ << qMakePair( pos, c );
}

QGradient* LinearGradient::toQGradient( const QRectF& rect ) const
{
        QLinearGradient* gr = new QLinearGradient(rect.left() + start_.x()*rect.width(),
                                                  rect.top()  + start_.y()*rect.height(),
                                                  rect.left() + final_.x()*rect.width(),
                                                  rect.top()  + final_.y()*rect.height());
        gr->setStops( colorStops_ );

        qDebug() << "linearGradient" << gr->start() << gr->finalStop();
        return gr;
}

void RadialGradient::setCenterPoint( const QPointF& p )
{
        center_ = p;
        ensureRange( center_ );
}

void RadialGradient::setFocalPoint( const QPointF& p )
{
        focal_ = p;
        ensureRange( focal_ );
}

void RadialGradient::setRadius( qreal r )
{
        radius_ = r;
        ensureRange( radius_ );
}

QGradient* RadialGradient::toQGradient( const QRectF& rect ) const
{
        qreal rad = sqrt( rect.width()*rect.width() + rect.height()*rect.height() );
        rad *= radius_;
        
        QRadialGradient* gr = new QRadialGradient(rect.left() + center_.x()*rect.width(),
                                                  rect.top()  + center_.y()*rect.height(),
                                                  rad,
                                                  rect.left() + focal_.x()*rect.width(),
                                                  rect.top()  + focal_.y()*rect.height() );
        
        gr->setStops( colorStops_ );

        return gr;
}


