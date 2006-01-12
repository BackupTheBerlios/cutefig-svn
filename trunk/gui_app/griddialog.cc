 
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

#include "griddialog.h"
#include "layouter.h"
#include "canvasview.h"

#include <QDoubleSpinBox>
#include <QLayout>
#include <QPushButton>

#include <QDebug>

GridDialog::GridDialog( double gridWidth, double snapWidth, CanvasView* cview )
        : QDialog( cview )
{
        setWindowTitle( tr("Adjust snap grid") );

        QDoubleSpinBox* gridBox = new QDoubleSpinBox( this );
        gridBox->setValue( gridWidth );
        gridBox->setSingleStep( 0.5 );
        gridBox->setMaximum( cview->paperWidth()/2 );

        QDoubleSpinBox* snapBox = new QDoubleSpinBox( this );
        snapBox->setValue( snapWidth );
        snapBox->setSingleStep( 0.5 );
        snapBox->setMaximum( cview->paperWidth()/2 );

        QVBoxLayout* mainLayout = new QVBoxLayout( this );

        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("&Grid width"), gridBox )
                .stretch()
                .labeledWidget( tr("&Snap width"), snapBox )
                .finishTo( mainLayout );
        
        QPushButton* ok = new QPushButton( tr("&Ok"), this );
        ok->setAutoDefault( true );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        
        QPushButton* cancel = new QPushButton( tr("Cancel") );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
        
        QHBoxLayout* bl = new QHBoxLayout();
        bl->addStretch();
        bl->addWidget( ok );
        bl->addWidget( cancel );

        mainLayout->addLayout( bl );

        connect( gridBox, SIGNAL( valueChanged(double) ), cview, SLOT( setGridWidth(double) ) );
        connect( snapBox, SIGNAL( valueChanged(double) ), cview, SLOT( setSnapWidth(double) ) );
}
