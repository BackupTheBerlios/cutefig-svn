 
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

#ifndef colorbutton_h
#define colorbutton_h

#include <QPushButton>
#include <QPixmap>

class QColor;


class ColorButton : public QPushButton
{
        Q_OBJECT
public:
        ColorButton( QWidget * parent =0 );
        ColorButton( const QColor &c, QWidget * parent =0 );
        ~ColorButton() { };

        void setColor( const QColor &c );

        QColor getColor() const { return color_; };

signals:
        void colorChanged( QColor );

public slots:
        void changeColor();

private:
        QColor color_;

        void draw();
    
        void resizeEvent( QResizeEvent* );
    
        QPixmap pixmap_;
};

#endif
