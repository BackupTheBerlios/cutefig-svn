 
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


#ifndef scaleaction_h
#define scaleaction_h

#include "interactiveaction.h"

#include <QMatrix>

//! Scales a DrawObject
/*! In order to do the scaling the following things have to be implemented:
 *     - decision whether the cursor is on a "scale handle" and which
 *     - handle the mouse movement, during scaling
 *     - do the actual scaling
 *
 *  According to the scale handle hit, we decide which kind of scaling
 *  is to be done. This decision is cached by having a pointer to a
 *  memberfunction put on the appropriate function.
 */
class ScaleAction : public InteractiveAction
{
        Q_OBJECT
public:
        ScaleAction( Controler* parent );

        virtual void click( const QPoint& p, Fig::PointFlags f, const QMatrix* m );
        virtual void move( const QPoint& p, const QMatrix* m );
        virtual bool wouldHandle( DrawObject* o, const QPoint& p = QPoint(), const QMatrix* m=0 );
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* m = 0 );

        virtual bool isActive() const { return false; }
        virtual void reset();

        virtual const QString commandName() const { return "scale"; }

private:
        //! Finds the 
        bool findOpposite( const QPoint& p, const QMatrix* m, DrawObject* o = 0 );
        bool pointHit( const QPointF& p1, const QPointF& p2 , const QMatrix* m );
        bool calcScaleMatrix( const QPoint& p, const QPoint& handle, QMatrix& m );

        typedef void (ScaleAction::*scaleValuesFptr_t)( const QPoint& d, const QPoint& handle,
                                                        qreal* x, qreal* y );
        scaleValuesFptr_t scaleValuesFptr;

        void scaleValuesHor( const QPoint& p, const QPoint& h, qreal* x, qreal* y );
        void scaleValuesVer( const QPoint& p, const QPoint& h, qreal* x, qreal* y );
        void scaleValuesDiag( const QPoint& p, const QPoint& h, qreal* x, qreal* y );


        int edgePointIndex_;
        QPointF opposite_;

        bool notStarted_;

        Fig::PointFlags startFlag_;
};

#endif
