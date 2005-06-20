 
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

#include <QWidget>
#include <QStringList>
#include <QPixmap>

/** \class Ruler
 *  
 *  \brief This is the ruler for the canvas window.
 *
 */

class Ruler : public QWidget
{
        Q_OBJECT
public:
        Ruler( int l, Qt::Orientation o, QWidget * parent =0 );
//!< The constructor takes the length l and the Orientation o.
        ~Ruler() { };

public slots:
        void setValue( int v );    //!< Sets the value of the pointerposition.
        void setLength( int l );   //!< Sets the length of the ruler
        void setStart( int v );    //!< Sets the lowest visible position

        void setScale( double s ); //!< Sets the zoom scale 
        void setUnit( double u );  //!< Sets the length unit

protected:
        void paintEvent( QPaintEvent *e );

private:
        void updateRuler();
        void calcTickMarks();       //<! calculates the tick marks smartly

        Qt::Orientation o_;
        int value_, oldValue_;
        int length_;
        double scale_;
        double unit_;
        double tickVal_;

        int ticks_, subTicks_, startTick_, startVal_, startPix_;
        static int tLen_, stLen_; //!< length of the (sub)ticks

        QStringList tickMarks_;

        QPixmap buffer_;
};

#endif
