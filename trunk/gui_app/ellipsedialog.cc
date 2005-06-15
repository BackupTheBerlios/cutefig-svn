 
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


#include <QtGui>

#include "controler.h"
#include "ellipsedialog.h"
#include "ellipse.h"
#include "numberlineedit.h"
#include "progutils.h"
#include "editdialogaction.h"

EllipseDialog::EllipseDialog( DrawObject* o, EditdialogAction* a,
                              QWidget * parent )
        : ObjectDialog( o, a, parent )
{
        qDebug("EllipseDialog::EllipseDialog");
        setCaption( tr("Ellipse Properties") );
        ellipse_ = (Ellipse*) o;
        setUpAll();
}


void EllipseDialog::setUpPrivate()
{
        qDebug("EllipseDialog::setUpPrivate");
        QFrame* page = new QFrame();
        tabWidget->addTab( page, "&Ellipse" );

        QGridLayout* topLayout = new QGridLayout( page );
//        ProgUtils::prepareLayout( topLayout, true );

        QGroupBox* angleGroup = new QGroupBox( tr("&Angle"), page );
        QHBoxLayout* angleLayout = new QHBoxLayout( angleGroup );
        angleSlider = new QSlider( -45,45, 1, 0, Qt::Horizontal, angleGroup );
        angleSlider->setTickmarks( QSlider::TicksBothSides );
        angleSlider->setTickInterval( 5 );
        angleLineEdit = new NumberLineEdit( 0, angleGroup );
        angleLineEdit->setValidator( new QIntValidator( -45,45,
                                                        angleLineEdit ) );
        angleLayout->addWidget( angleSlider );
        angleLayout->addWidget( angleLineEdit );
        
        QGroupBox* stg = new QGroupBox( tr("&Type"), page );
        QVBoxLayout* stl = new QVBoxLayout( stg );
        subType = new QButtonGroup( this );
        subType->setExclusive( true );
        QRadioButton* ellipseButton = new QRadioButton( "&Ellipse", stg );
        QRadioButton* circleButton = new QRadioButton( "&Circle", stg );
        subType->addButton( ellipseButton );
        subType->addButton( circleButton );
        stl->addWidget( ellipseButton );
        stl->addWidget( circleButton );


        QGroupBox* dg = new QGroupBox( tr("Specified &by"), page );
        definition = new QButtonGroup( this );
        definition->setExclusive( true );
        QRadioButton* radiiButton = new QRadioButton( "&Radii", dg );
        QRadioButton* diamButton = new QRadioButton( "&Radii", dg );
        definition->addButton( radiiButton );
        definition->addButton( diamButton );
        QVBoxLayout* dgl = new QVBoxLayout( dg );
        dgl->addWidget( radiiButton );
        dgl->addWidget( diamButton );

        topLayout->addMultiCellWidget( angleGroup, 0,0, 0,1 );
        topLayout->addWidget( stg, 1,0 );
        topLayout->addWidget( dg, 1,1 );
}

void EllipseDialog::setDefaultValues()
{
        angleSlider->setValue( (int) ellipse_->angle() );

        ObjectDialog::setDefaultValues();
}

void EllipseDialog::setUpConnections()
{
        qDebug("Ellipse::setupConnections");
        
        connect( angleSlider, SIGNAL( valueChanged(int) ),
                 angleLineEdit, SLOT( setValue(int) ) );
        connect( angleSlider, SIGNAL( valueChanged(int) ),
                 ellipse_, SLOT( setAngle(int) ) );
        connect( angleSlider, SIGNAL( valueChanged(int) ),
                 action_, SLOT( wObjectHasChanged() ) );

        ObjectDialog::setUpConnections();
}

