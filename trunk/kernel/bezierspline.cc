 
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

#include "bezierspline.h"
#include "outputbackend.h"

#include <QPainter>
#include <QPainterPath>

BezierSpline::BezierSpline( Figure* parent )
        : DrawObject( parent ),
          finished_( false ),
          oppositeControlPoint_( -1 )
{}

bool BezierSpline::pointHitsOutline( const QPointF& p, qreal tolerance ) const
{
        return false;
}

void BezierSpline::outputToBackend( OutputBackend *ob )
{
        ob->outputBezierSpline( this );
}

void BezierSpline::setupPainterPath()
{
        int s = points_.size()-1;
        
                           
        int m = s % 3;
        s -= m;

//        qDebug() << "setupPainterPath" << s << m;
        
        painterPath_ = QPainterPath();
        painterPath_.moveTo( points_[0] );
        
        for ( int i = 1; i < s; )
                painterPath_.cubicTo( points_[i++], points_[i++], points_[i++] );

        if ( m == 2 )
                painterPath_.quadTo( points_[s++], points_[s++] );
}

void BezierSpline::drawTentative( QPainter* p, const QPen& auxPen ) const
{
        p->drawPath( painterPath_ );

//        qDebug () << "drawTentative" << currentPointIndex() << oppositeControlPoint_;
        if ( oppositeControlPoint_ == -1 )
                return;
        
        QPen op = p->pen();
        p->setPen( auxPen );
        p->drawLine( points_[currentPointIndex()], points_[oppositeControlPoint_] );
        p->setPen( op );
}

void BezierSpline::passPointFlag( Fig::PointFlag f )
{
        if ( f & Fig::Final )
                finished_ = true;
}

int BezierSpline::nextPointIndex()
{
        if ( finished_ )
                return -1;

        points_ << QPointF();        

        findOppositeControlPoint( points_.size() );
        
        if ( oppositeControlPoint_ != -1 )
                points_.insert( oppositeControlPoint_, QPointF() );

        return points_.size() - 1;
}


void BezierSpline::findOppositeControlPoint( int current )
{
        qDebug() << "findOppositeControlPoint" << current;
        if ( current % 3 == 0 ) 
                oppositeControlPoint_ = -1;
        
        else if ( current % 2 )
                oppositeControlPoint_ = current - 2;
        else
                oppositeControlPoint_ = current + 2;

        qDebug() << oppositeControlPoint_ << points_.size();
        
        if ( oppositeControlPoint_ > points_.size() )
                oppositeControlPoint_ = -1;
}
