 
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

#include "rectangloiddialog.h"
#include "ellipse.h"
#include "rectangle.h"
#include "layouter.h"
#include "chainbutton.h"

#include <QtGui>

RectangloidDialog::RectangloidDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : ObjectDialog( o,a, parent )
{
        QWidget* page = new QWidget;
        tabWidget()->addTab( page, o->objectname() );

        pageLayout_ = new QVBoxLayout( page );
        
        QGroupBox* angleGroup = new QGroupBox( tr("&Angle") );
        
        
        QHBoxLayout* angleLayout = new QHBoxLayout( angleGroup );
        
        angleSlider_ = new QSlider( Qt::Horizontal );
        angleSlider_->setMinimum( -45 );
        angleSlider_->setMaximum(  45 );
        angleSlider_->setSingleStep( 1 );
        angleSlider_->setTickPosition( QSlider::TicksBothSides );
        angleSlider_->setTickInterval( 5 );

        angleSpin_ = new QSpinBox;
        angleSpin_->setRange( -45, 45 );
        angleSpin_->setSingleStep( 1 );
        
        angleLayout->addWidget( angleSpin_ );
        angleLayout->addWidget( angleSlider_ );

        connect( angleSpin_, SIGNAL( valueChanged(int) ), angleSlider_, SLOT( setValue(int) ) );
        connect( angleSlider_, SIGNAL( valueChanged(int) ), angleSpin_, SLOT( setValue(int) ) );
        connect( angleSlider_, SIGNAL( valueChanged(int) ), this, SLOT( noticeChange() ) );

        pageLayout_->addWidget( angleGroup );
}



EllipseDialog::EllipseDialog( DrawObject* o, EditdialogAction* a,
                              QWidget * parent )
        : RectangloidDialog( o, a, parent )
{
        qDebug("EllipseDialog::EllipseDialog");

        QGroupBox* stg = new QGroupBox( tr("&Type") );
        QGroupBox* dg = new QGroupBox( tr("Specified &by") );
        
        subType_ = new QButtonGroup;
        subType_->setExclusive( true );
        QRadioButton* ellipseButton = new QRadioButton(tr("&Ellipse"));
        QRadioButton* circleButton = new QRadioButton(tr("&Circle"));
        subType_->addButton( ellipseButton, 0 );
        subType_->addButton( circleButton, 1 );
        QVBoxLayout* stl = new QVBoxLayout( stg );
        stl->addWidget( ellipseButton );
        stl->addWidget( circleButton );
        connect( subType_, SIGNAL( buttonClicked(int) ), this, SLOT( noticeChange() ) );
        
        definition_ = new QButtonGroup;
        definition_->setExclusive( true );
        QRadioButton* radiiButton = new QRadioButton(tr("&Radii"));
        QRadioButton* diamButton = new QRadioButton(tr("&Diameter"));
        definition_->addButton( diamButton, 0 );
        definition_->addButton( radiiButton, 1 );
        QVBoxLayout* dgl = new QVBoxLayout( dg );
        dgl->addWidget( radiiButton );
        dgl->addWidget( diamButton );
        connect( definition_, SIGNAL( buttonClicked(int) ), this, SLOT( noticeChange() ) );

        QHBoxLayout* ell = new QHBoxLayout;
        ell->addWidget( stg );
        ell->addWidget( dg );

        pageLayout()->addLayout( ell );
}

void EllipseDialog::commitChangesPrivate()
{
        ellipse_->setAngle( angleSlider()->value() );
        ellipse_->setCircle( subType_->checkedId() );
        ellipse_->setSpecifiedByRadii( definition_->checkedId() );
}


void EllipseDialog::setupInitialValuesPrivate()
{
        angleSlider()->setValue( qRound( ellipse_->angle() ) );
        subType_->button( ellipse_->isCircle() ? 1 : 0 )->setChecked( true );
        definition_->button( ellipse_->isSpecifiedByRadii() ? 1 : 0 )->setChecked( true );
}


void EllipseDialog::castDrawObject()
{
        ellipse_ = static_cast<Ellipse*>( drawObject() );
}


template<>
ObjectDialog* TObjectGUIHandler<Ellipse>::makeEditDialog( DrawObject* o, EditdialogAction* a, 
                                                          QWidget* parent )
{
        return new EllipseDialog( o, a, parent );
}



RectangleDialog::RectangleDialog( DrawObject* o, EditdialogAction* a, QWidget* parent )
        : RectangloidDialog( o,a, parent )
{
        QGroupBox* corners = new QGroupBox( tr("Rounded &corners") );

        roundedCorners_ = new QCheckBox( tr("Rounded corners") );
        xCornerRadius_ = new QDoubleSpinBox;
	xCornerRadius_->setSingleStep( 0.1 );
	xCornerRadius_->setMinimum( 0.0 );
        yCornerRadius_ = new QDoubleSpinBox;
	yCornerRadius_->setSingleStep( 0.1 );
	yCornerRadius_->setMinimum( 0.0 );
        equalCornerRadii_ = new ChainButton;
        equalCornerRadii_->setCheckable( true );
        
        Layouter( new QHBoxLayout( corners ) )
                .widget( roundedCorners_ )
                .labeledWidget( tr("X corner radius"), xCornerRadius_ )
                .widget( equalCornerRadii_ )
                .labeledWidget( tr("Y corner radius"), yCornerRadius_ )
                .finishTo( corners );

        connect( roundedCorners_, SIGNAL( clicked() ), this, SLOT( noticeChange() ) );
        connect( equalCornerRadii_, SIGNAL( clicked() ), this, SLOT( noticeChange() ) );
        connect( xCornerRadius_, SIGNAL( valueChanged(double) ), this, SLOT( noticeChange() ) );
        connect( yCornerRadius_, SIGNAL( valueChanged(double) ), this, SLOT( noticeChange() ) );

	pageLayout()->addWidget( corners );
}

void RectangleDialog::commitChangesPrivate()
{
	rectangle_->setAngle( angleSlider()->value() );

        bool rc = roundedCorners_->isChecked();
        rectangle_->setRoundedCorners( rc );

	qDebug() << __PRETTY_FUNCTION__ << rc;
	qDebug() << __PRETTY_FUNCTION__ << sender();

	

        setRoundedCornersEnabled( rc );
        
        if ( rc ) {
		bool eq = equalCornerRadii_->isChecked();
 		if ( eq )
 			if ( sender() == xCornerRadius_ )
 				yCornerRadius_->setValue( xCornerRadius_->value() );
 			else if ( sender() == yCornerRadius_ )
 				xCornerRadius_->setValue( yCornerRadius_->value() ); 	
		
                qDebug() << "setting radii" << xCornerRadius_->value() << yCornerRadius_->value();
                rectangle_->setXCornerRad( xCornerRadius_->value() );
                rectangle_->setYCornerRad( yCornerRadius_->value() );
		
                rectangle_->setEqualRoundedCorners( eq );
		
		yCornerRadius_->setValue( rectangle_->yCornerRad() );
		xCornerRadius_->setValue( rectangle_->xCornerRad() );

		xCornerRadius_->setMaximum( rectangle_->maxXCornerRad() );
		yCornerRadius_->setMaximum( rectangle_->maxYCornerRad() );
        }
}

void RectangleDialog::setupInitialValuesPrivate()
{
        angleSlider()->setValue( qRound( rectangle_->angle() ) );

        bool rc = rectangle_->hasRoundedCorners();
        setRoundedCornersEnabled( rc );
        roundedCorners_->setChecked( rc );

        if ( rc ) {
                xCornerRadius_->setValue( rectangle_->xCornerRad() );
                yCornerRadius_->setValue( rectangle_->yCornerRad() );
        }

	xCornerRadius_->setMaximum( rectangle_->maxXCornerRad() );
	yCornerRadius_->setMaximum( rectangle_->maxYCornerRad() );

        equalCornerRadii_->setChecked( rectangle_->hasEqualCornerRadii() );
}

void RectangleDialog::updateRadiiSpinBoxes( double v )
{
	if ( !rectangle_->hasEqualCornerRadii() )
		return;
	
	if ( sender() == xCornerRadius_ )
		yCornerRadius_->setValue( v );
	else
		xCornerRadius_->setValue( v );
}

void RectangleDialog::setRoundedCornersEnabled( bool e )
{
        xCornerRadius_->setEnabled( e );
        yCornerRadius_->setEnabled( e );
        equalCornerRadii_->setEnabled( e );    
}


void RectangleDialog::castDrawObject()
{
        rectangle_ = static_cast<Rectangle*>( drawObject() );
}


template<>
ObjectDialog* TObjectGUIHandler<Rectangle>::makeEditDialog( DrawObject* o, EditdialogAction* a, 
                                                            QWidget* parent )
{
        return new RectangleDialog( o, a, parent );
}
