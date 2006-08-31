 
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

#include "createaction.h"
#include "addcommand.h"
#include "allobjects.h"

#include <QPainter>

void CreateAction::click( const QPoint& p, Fig::PointFlags f, const QMatrix* m )
{
        firstClick_ = true;
        
        if ( !cObject_->pointSet( m->map( QPointF( p ) ), f ) ) {
                controler_->execAction( new AddCommand( selection_ ) );
                selection_.updateBackups();
        }
}


void CreateAction::move( const QPoint& p, const QMatrix* m )
{        
        cObject_->cursorMove( m->map( QPointF( p ) ) );
        if ( firstClickDone() )
                changeStatusMove();
}


void CreateAction::handleSelection()
{
        setInitialStatus();
        
        cObject_ = createObject();
        selection_.setObjectToBeCreated( cObject_ );
}

void CreateAction::drawMetaData( QPainter* p, const ViewBase* v ) const
{
        if ( cObject_ ) {
                v->drawObjectsPoints( p, cObject_ );
                p->setPen( Qt::red );
                v->drawObjectsMetaData( p, cObject_ );
        }
}

void CreateAction::reset()
{
        cObject_ = 0;
        firstClick_ = false;
}

template<> void TCreateAction<Rectangle>::init()
{
        setShortcut( Qt::Key_R );
}



template<> void TCreateAction<Ellipse>::init()
{
        qDebug() << "TCreateAction<Ellipse>::init()";
        setShortcut( Qt::Key_E );
}

template<> void TCreateAction<Ellipse>::setInitialStatus_private()
{
        status_.setInformation( Qt::NoModifier,
                                ActionStatus::Information()
                                .setLeft( tr("corner point") )
                                .setHelp( tr("ellipse by diameter") ) );
        
        status_.setInformation( Qt::AltModifier,
                                ActionStatus::Information()
                                .setLeft( tr("center point") )
                                .setHelp( tr("ellipse by radii") ) );

        status_.setInformation( Qt::ShiftModifier,
                                ActionStatus::Information()
                                .setLeft( tr("point on circle") )
                                .setHelp( tr("circle by diameter") ) );
        
        status_.setInformation( Qt::AltModifier,
                                ActionStatus::Information()
                                .setLeft( tr("center point") )
                                .setHelp( tr("circle by radius") ) );

        status_.setInformation( Qt::ControlModifier,
                                ActionStatus::Information()
                                .setLeft( tr("axis point") )
                                .setHelp( tr("angled ellipse") ) );
}

template<> void TCreateAction<Ellipse>::changeStatusClick() 
{
}

template<> void TCreateAction<Ellipse>::changeStatusMove() 
{
        QRectF r = drawObject()->controlPointRect();
        double u = drawObject()->figure().unit();
        QString s;
        QTextStream ts( &s );
        ts.setRealNumberPrecision( 2 );
        ts.setRealNumberNotation( QTextStream::FixedNotation );

        ts << tr("W: ") << r.width()/u << tr(" H: ") << r.height()/u;

        status_.setStatus( s );
        emit statusChanged( status_ );
}






template<> void TCreateAction<Polygon>::init()
{
        setShortcut( Qt::Key_P );
}

template<> void TCreateAction<Polyline>::init()
{
        setShortcut( Qt::Key_L );
}

template<> void TCreateAction<Arc>::init()
{
        setShortcut( Qt::Key_A );
}

template<> void TCreateAction<BezierSpline>::init()
{
        setShortcut( Qt::Key_B );
}

template<> void TCreateAction<TextObject>::init()
{
        setShortcut( Qt::Key_T );
}
