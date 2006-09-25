 
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

#ifndef ruler_h
#define ruler_h

#include "typedefs.h"
#include "length.h"

#include <QFrame>
#include <QStringList>
#include <QPixmap>
#include <QLabel>

class RulerDispatcher;

//! This is the ruler for the canvas window.
/*! This is the ruler for the canvas view. It shows ticks and marks
 *  them. Depending on zoom and length scale the ticks and the
 *  tickmarks are calculated in a suitable way by calcTickMarks(). It
 *  indicates the current pointerposition by a red line. It can be
 *  drawn vertically or horiziontally depending on the orientation
 *  given in the constructor.
 *
 *  It is also planned that helplines can be pulled out of the ruler.
 *
 */
class Ruler : public QFrame
{
        Q_OBJECT
public:
        friend class RulerDispatcher;
        
        Ruler( int l, int width_, Qt::Orientation o, QWidget * parent =0 );
//!< The constructor takes the length l and the Orientation o.
        ~Ruler() { };

private:
        void setValue( int v );    //!< Sets the value of the pointerposition.
        void setLength( int l );   //!< Sets the length of the ruler

        void setUnit( const ResourceKey& k );   //!< Sets the length unit

        void setIndicating( bool indicating );
	void setZoomScale( double s ); //!< Sets the zoomScale 
        void setStart( int v );    //!< Sets the lowest visible position

	void setOffset( double o );


        void paintEvent( QPaintEvent *e );

        void contextMenuEvent( QContextMenuEvent* e );

        void updateRuler();
        void calcTickMarks();       //<! calculates the tick marks smartly

        Qt::Orientation o_;
        int value_, oldValue_;
        int length_;
        double zoomScale_;
        ResourceUser<Length> unit_;
        double tickVal_;

        double ticks_, subTicks_, offset_;
	int startPos_;

	int rulerWidth_, frameSpace_;
	
        QStringList tickMarks_;

        bool indicating_;
        
        QPixmap buffer_;
};


class RulerDispatcher : public QObject
{
        Q_OBJECT
public:
	RulerDispatcher( const QSize& s, QObject* parent = 0 );

	Ruler* verticalRuler() const { return vertical_; }
	Ruler* horizontalRuler() const { return horizontal_; }
        QLabel* unitLabel() const { return unitLabel_; }

public slots:
        void setUnit( const ResourceKey& k );
	void verticalScroll( int s );
	void horizontalScroll( int s );
	void sizeChange( const QSize& s );
	void setMatrix( const QMatrix& m );
	void setPos( const QPoint& p );
	void setIndicating( bool i );

private:
        int rulerWidth_;
        
	Ruler* vertical_;
	Ruler* horizontal_;

        QLabel* unitLabel_;
};

#endif
