 
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
        Ruler( int l, Qt::Orientation o, QWidget * parent =0 );
//!< The constructor takes the length l and the Orientation o.
        ~Ruler() { };

        void setValue( int v );    //!< Sets the value of the pointerposition.
        void setLength( int l );   //!< Sets the length of the ruler

        void setScale( double s ); //!< Sets the zoom scale 
        void setUnit( const ResourceKey& k );   //!< Sets the length unit

        void setIndicating( bool indicating );
        
public slots:
        void setStart( int v );    //!< Sets the lowest visible position

protected:
        void paintEvent( QPaintEvent *e );

        void contextMenuEvent( QContextMenuEvent* e );
        

private:
        void updateRuler();
        void calcTickMarks();       //<! calculates the tick marks smartly

        Qt::Orientation o_;
        int value_, oldValue_;
        int length_;
        double scale_;
        ResourceUser<Length> unit_;
        double tickVal_;

        double ticks_, subTicks_, startTick_;
        int startVal_, startPix_;
        static int tLen_, stLen_; //!< length of the (sub)ticks

        QStringList tickMarks_;

        bool indicating_;
        
        QPixmap buffer_;
};

#endif
