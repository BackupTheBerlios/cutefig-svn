 
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

#ifndef lineshowwidget_h
#define lineshowwidget_h

#include <qwidget.h>
#include "pen.h"
#include "reslib.h"

class QColor;
class QSize;

class LineShowWidget : public QWidget
{
        Q_OBJECT
public:
        LineShowWidget( const Pen &p, 
                        const int h, 
                        QWidget * parent =0 );

        ~LineShowWidget() { };

public slots:
        void setColor( QColor c );
        void setWidth( double w );
        void setDashes( int d );
        void setFlatCap() { capStyle_ = Qt::FlatCap; myUpdate(); }
        void setSquareCap() { capStyle_ = Qt::SquareCap; myUpdate(); }
        void setRoundCap() { capStyle_ = Qt::RoundCap; myUpdate(); }
        
        void setBevelJoin() { joinStyle_ = Qt::BevelJoin; myUpdate(); }
        void setMiterJoin() { joinStyle_ = Qt::MiterJoin; myUpdate(); }
        void setRoundJoin() { joinStyle_ = Qt::RoundJoin; myUpdate(); }

        void setCapStyle( int cs );
        void setJoinStyle( int js );

signals:
        void changed( const Pen& );

private:
        Pen pen_;
        int capStyle_, joinStyle_;

        void myUpdate();

        void paintEvent( QPaintEvent * );

};

#endif
