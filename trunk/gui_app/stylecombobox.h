 
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

#ifndef stylecombobox_h
#define stylecombobox_h

#include <QComboBox>
#include <QPixmap>

#include <QVector>

#include "pen.h"
#include "reslib.h"

class StyleComboBox : public QComboBox
{
        Q_OBJECT
public:
        StyleComboBox( QWidget * parent =0 );
        ~StyleComboBox() { }

        void setCurrentIndex( int index );

signals:
        void activatedDash( int );
        void highlightedDash( int );

private:        
        QPixmap drawItemQtPen( Qt::PenStyle ps );
        QPixmap drawItemCustomPen( int dkey );

private slots:
        void em_activated( int );
        void em_highlighted( int );
};

#endif
