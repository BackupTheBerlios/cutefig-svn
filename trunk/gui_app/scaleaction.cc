 
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

#include "scaleaction.h"
#include "selection.h"
#include "changecommand.h"
#include "drawobject.h"
#include "geometry.h"

#include <QRect>
#include <QMatrix>

#include <QDebug>


ScaleAction::ScaleAction( Controler* parent, QActionGroup* group )
        : InteractiveAction( parent, group )
{
        setText( tr("&Scale") );
        setShortcut( Qt::ALT+Qt::Key_S );
        reset();
}


void ScaleAction::click( const QPoint& p, Fig::PointFlag f, const QMatrix* m )
{
        if ( notStarted_ ) {
                findOpposite( p, m );
                QRectF br = selection_.boundingRect();
                aspectRatio_ *= br.height() / br.width();
                last_ = lastUncorr_ = p;
                notStarted_ = false;
                startFlag_ = f;
                return;
        }

        controler_->execAction( new ChangeCommand( selection_ ) );
        selection_.updateBackups();
        reset();
}

void ScaleAction::move( const QPoint& _p, const QMatrix* _m )
{
        QMatrix m;
        QPointF c = selection_.boundingRect().center();

        QPoint p;

        if ( startFlag_ & Fig::Straight ) 
                p = _p;
        else {
                QPoint d = _p - lastUncorr_;

                if ( abs(d.x()) >= abs(d.y()) ) 
                        d.setY( qRound( d.x() * aspectRatio_ ) );
                else 
                        d.setX( qRound( d.y() / aspectRatio_ ) );

                p = last_ + d;
                lastUncorr_ = _p;
        }

        if ( calcScaleMatrix( p,m ) ) {
                foreach ( DrawObject* o, selection_.objects() ) {
                        o->move( -c );
                        o->mapMatrix( m );
                        c += _m->map( diff_ );
                        o->move( c );
                }
                last_ = p;
        }
}

bool ScaleAction::calcScaleMatrix( const QPoint& p, QMatrix& m )
{
        qreal qx = 0;
        qreal qy = 0;

        (this->*scaleValuesFptr)( p, qx,qy );

        if ( startFlag_ & Fig::Special1 ) {
                qx *= 2.;
                qy *= 2.;
                opposite_ -= diff_;
                diff_ = QPointF( 0., 0. );
        } else
                diff_ /= 2;

        qx += 1.;
        qy += 1.;

        if ( fabs(qx * qy) > 1e-12 ) {
                m.scale( qx, qy );
                return true;
        } else
                return false;
}


void ScaleAction::scaleValuesHor( const QPoint& p, qreal& x, qreal& )
{
        int d = p.x() - last_.x();
        x = qreal( d ) / ( last_.x() - opposite_.x() );
        diff_.setX( d );
}

void ScaleAction::scaleValuesVer( const QPoint& p, qreal&, qreal& y )
{
        int d = p.y() - last_.y();
        y = qreal( d ) / ( last_.y() - opposite_.y() );
        diff_.setY( d );
}


void ScaleAction::scaleValuesDiag( const QPoint& p, qreal& x, qreal& y )
{
        scaleValuesHor( p, x,y );
        scaleValuesVer( p, x,y );
}


bool ScaleAction::findOpposite( const QPoint& p, const QMatrix* m, DrawObject* o )
{
        QRectF br = o ? o->boundingRect() : selection_.boundingRect();

        QVector<QPointF> points = Geom::boundingPoints( br );

        int i = 0;
        while ( i < 8 && !pointHit( p, points[i], m )  )
                ++i;

        if ( i == 8 || points[4].isNull() ) 
                return false;

        opposite_ = points[(i%2) ? i-1 : i+1] * m->inverted();


        if ( i < 4 ) {
                scaleValuesFptr = &ScaleAction::scaleValuesDiag;
                if ( i < 2 ) {
                        aspectRatio_ = 1.;
                        cursor_ = Qt::SizeFDiagCursor;
                } else {
                        aspectRatio_ = -1.;
                        cursor_ = Qt::SizeBDiagCursor;
                }
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
        return ( Geom::distance( p1, p2 * m->inverted() ) < 5 );
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
        last_ = QPoint();
        notStarted_ = true;
}
