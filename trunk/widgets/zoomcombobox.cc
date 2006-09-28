 
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

#include "zoomcombobox.h"
#include "percentvalidator.h"

#include <QKeyEvent>

#include <QDebug>

ZoomComboBox::ZoomComboBox( QWidget* parent )
        : QComboBox( parent )
{
        setEditable( true );
        setInsertPolicy( QComboBox::NoInsert );
        
        setValidator( new PercentValidator( this ) );

        connect( this, SIGNAL( activated( int ) ), this, SLOT( comboBoxChanged( int ) ) );
}

void ZoomComboBox::changeZoom( double value )
{
        int index =  findData( value );
        
        if ( index != -1 )
                setCurrentIndex( index );
        else 
             setEditText( QString("%1 %").arg( value*100, 0, 'g', 3 ) );
}

void ZoomComboBox::comboBoxChanged( int id )
{
	emit zoomChanged( id - 2 );
}

void ZoomComboBox::textChanged()
{
        int pos;
        QString s = currentText();
        if ( validator()->validate( s, pos ) == QValidator::Acceptable ) {
                s.chop( 1 );
                emit zoomChanged( s.toDouble() / 100 );
        }
}

void ZoomComboBox::keyPressEvent( QKeyEvent* ke )
{
        if ( ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter )
                textChanged();

        QComboBox::keyPressEvent( ke );
}
