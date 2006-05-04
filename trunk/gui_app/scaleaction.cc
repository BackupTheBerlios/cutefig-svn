 
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

#include "scaleaction.h"
#include "selection.h"
#include "changecommand.h"
#include "drawobject.h"
#include "geometry.h"

#include <cmath>

#include <QRect>

#include <QDebug>


ScaleAction::ScaleAction( Controler* parent )
        : InteractiveAction( parent )
{
        setText( tr("&Scale") );
        setShortcut( Qt::ALT+Qt::Key_S );
        reset();
}

/*!
 */
void ScaleAction::click( const QPoint&, Fig::PointFlags f, const QMatrix* )
{
        if ( notStarted_ ) {
                notStarted_ = false;
                startFlag_ = f;
                if ( f & Fig::Special1 )
                        opposite_ = selection_.boundingRect().center();
        } else {
                controler_->execAction( new ChangeCommand( selection_ ) );
                selection_.updateBackups();
                reset();
        }
}

void ScaleAction::move( const QPoint& p, const QMatrix* m )
{
        QPointF h = Geom::boundingPoints(selection_.boundingRect())[edgePointIndex_];
        
        QPoint handle = m->map( h ).toPoint();


        QMatrix newMatrix;

        if ( calcScaleMatrix( p, handle, newMatrix ) ) {
                foreach ( DrawObject* o, selection_.objects() ) {
                        o->move( -opposite_ );
                        o->mapMatrix( newMatrix );
                        o->move( opposite_ );
                }
        }
}

bool ScaleAction::calcScaleMatrix( const QPoint& p, const QPoint& handle, QMatrix& m )
{
        qreal qx = 0;
        qreal qy = 0;

        (this->*scaleValuesFptr)( p, handle, &qx, &qy );

        qx += 1.;
        qy += 1.;

        if ( qx <= 0 || qy <= 0 )
                return false;

        if ( !(startFlag_ & Fig::Straight) & edgePointIndex_ < 4 ) {
                
                bool isBigger = qx > 1. && qy > 1;
                
                if ( (qx < qy) ^ isBigger )
                        qy = qx;
                else
                        qx = qy;
        }                

        if ( fabs(qx * qy) > 1e-12 ) {
                m.scale( qx, qy );
                return true;
        } else
                return false;
}


void ScaleAction::scaleValuesHor( const QPoint& p, const QPoint& h, qreal* x, qreal* )
{
        int d = p.x() - h.x();
        *x = qreal( d ) / ( h.x() - opposite_.x() );
}

void ScaleAction::scaleValuesVer( const QPoint& p, const QPoint& h, qreal*, qreal* y )
{
        int d = p.y() - h.y();
        *y = qreal( d ) / ( h.y() - opposite_.y() );
}


void ScaleAction::scaleValuesDiag( const QPoint& p, const QPoint& h, qreal* x, qreal* y )
{
        scaleValuesHor( p,h, x,y );
        scaleValuesVer( p,h, x,y );
}


bool ScaleAction::findOpposite( const QPoint& p, const QMatrix* m, DrawObject* o )
{
        QRectF br = o ? o->boundingRect() : selection_.boundingRect();

        QVector<QPointF> points = Geom::boundingPoints( br );

        int& i = edgePointIndex_ = 0;
        
        while ( i < 8 && !pointHit( p, points[i], m )  )
                ++i;

        if ( i == 8 || points[4].isNull() ) 
                return false;

        opposite_ = points[(i%2) ? i-1 : i+1] * m->inverted();

        if ( i < 4 ) {
                scaleValuesFptr = &ScaleAction::scaleValuesDiag;
                if ( i < 2 )
                        cursor_ = Qt::SizeFDiagCursor;
                else
                        cursor_ = Qt::SizeBDiagCursor;
                
        }
        else  if ( i < 6 ) {
                scaleValuesFptr = &ScaleAction::scaleValuesHor;
                cursor_ = Qt::SizeHorCursor;
        } else {
                scaleValuesFptr = &ScaleAction::scaleValuesVer;
                cursor_ = Qt::SizeVerCursor;
        }
        
        return true;
}

bool ScaleAction::pointHit( const QPointF& p1, const QPointF& p2, const QMatrix* m )
{
        return Geom::isNear( p1, p2 * m->inverted() );
}

bool ScaleAction::wouldHandle( DrawObject* o, const QPoint& p, const QMatrix* m )
{
        if ( m )
                return findOpposite( p, m, o );
        else 
                return o;
}

bool ScaleAction::wouldHandleSelection( const QPoint& p, const QMatrix* m )
{
        if ( !notStarted_ )
                return true;

        if ( selection_.isEmpty() )
                return false;

        if ( !m )
                return true;

        return findOpposite( p, m );
}

void ScaleAction::reset()
{
        opposite_ = QPoint();
        notStarted_ = true;
}
