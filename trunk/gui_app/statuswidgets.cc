 
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

#include "statuswidgets.h"
#include "figure.h"

#include <QTextStream>


CoordWidget::CoordWidget( const Figure& fig, QWidget* parent )
        : QLabel( parent ),
          figure_( fig )
{
        setFrameStyle( QFrame::Panel | QFrame::Sunken );
        setMinimumWidth( QFontMetrics( QFont() ).width('0') * 20 );
}

void CoordWidget::setCoords( const QPoint& p )
{
        pos_ = p;
        updateContents();
}

void CoordWidget::setIndicating( bool ind )
{
        indicating_ = ind;
        updateContents();
}

void CoordWidget::updateContents()
{
        if ( !indicating_ ) {
                clear();
                return;
        }
        
        QString s;
        
        double x = double(pos_.x())/figure_.unit();
        double y = double(pos_.y())/figure_.unit();

        QTextStream ts( &s );
        ts.setRealNumberPrecision( 2 );
        ts.setRealNumberNotation( QTextStream::FixedNotation );

        ts << x << " : " << y << " " << figure_.unitKey().keyString();

        setText( s );
}
