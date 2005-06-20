 
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

class QMatrix;

class ScaleAction : public InteractiveAction
{
        Q_OBJECT
public:
        ScaleAction( Controler* parent, QActionGroup* group = 0 );
        ~ScaleAction() {}

        virtual void click( const QPoint& p, Fig::PointFlag f, const QMatrix* m );
        virtual void move( const QPoint& p, const QMatrix* m );
        virtual bool wouldHandle( DrawObject* o, const QPoint& p = QPoint(), const QMatrix* m=0 );
        virtual bool wouldHandleSelection( const QPoint& p = QPoint(), const QMatrix* m = 0 );

        virtual bool isActive() const { return false; }
        virtual void reset();

        virtual const QString commandName() const { return "scale"; }

private:
        bool findOpposite( const QPoint& p, const QMatrix* m, DrawObject* o = 0 );
        bool pointHit( const QPointF& p1, const QPointF& p2 , const QMatrix* m );
        bool calcScaleMatrix( const QPoint& p, QMatrix& m );

        typedef void (ScaleAction::*scaleValuesFptr_t)( const QPoint& d, qreal& x, qreal& y );
        scaleValuesFptr_t scaleValuesFptr;

        void scaleValuesHor( const QPoint& p, qreal& x, qreal& y );
        void scaleValuesVer( const QPoint& p, qreal& x, qreal& y );
        void scaleValuesDiag( const QPoint& p, qreal& x, qreal& y );

        // typedef bool (ScaleAction::*doScaleFptr_t)( const QPointF&p, QMatrix& m );
//         doScaleFptr_t doScaleFptr;



//         bool doScaleHor( const QPointF& p, QMatrix& m );
//         bool doScaleVer( const QPointF& p, QMatrix& m );
//         bool doScaleDiag( const QPointF& p, QMatrix& m );

        QPointF opposite_, diff_;
        qreal aspectRatio_;
        QPoint last_, lastUncorr_;
        QPolygonF origPoints_;
        bool notStarted_;

        Fig::PointFlag startFlag_;
};

#endif
