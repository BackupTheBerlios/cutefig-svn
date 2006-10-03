 
/*****************************************************************************
**
**  $Id$
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

#include "pixoutdialog.h"
#include "figure.h"
#include "colorbutton.h"
#include "chainbutton.h"
#include "layouter.h"

#include <QtGui>

#include <limits>

PixoutDialog::PixoutDialog( PIXOutput* filter, QWidget* parent )
        : EditDialog( parent ),
          filter_( filter ),
          keepAspectRatio_( true ),
          aspectRatio_( 1 )
{
        setWindowTitle( tr("CuteFig: export to a bitmap graphics:") );

	QGridLayout* mainLayout = new QGridLayout;
	
        QGroupBox* sizeGroup = new QGroupBox( tr("Size") );
	QHBoxLayout* sizeLayout = new QHBoxLayout( sizeGroup );

        xres_ = new QSpinBox;
        xres_->setRange( 0, std::numeric_limits<int>::max() );
        xres_->setSingleStep( 1 );
        
        yres_ = new QSpinBox;
        yres_->setRange( 0, std::numeric_limits<int>::max()  );
        yres_->setSingleStep( 1 );

	QVBoxLayout* whlt = new QVBoxLayout;
	Layouter( new QHBoxLayout ).labeledWidget( tr("&Width"), xres_ ).finishTo( whlt );
	Layouter( new QHBoxLayout ).labeledWidget( tr("&Height"), yres_ ).finishTo( whlt );
	sizeLayout->addLayout( whlt );
	
        keepAspect_ = new ChainButton;
	sizeLayout->addWidget( keepAspect_ );
	
        scale_ = new QDoubleSpinBox;
        scale_->setSuffix(" %");
        scale_->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        scale_->setSingleStep( 10 );

	Layouter( new QHBoxLayout ).labeledWidget( tr("&Scale"), scale_ ).finishTo( sizeLayout );

        mainLayout->addWidget( sizeGroup, 0,0, 1,3 );
	mainLayout->addItem( new QSpacerItem( 3*layout()->spacing(), 0 ), 1,1 );
	
	QColor bg( Qt::white );
        bg.setAlpha( 0 );
        bgColor_ = new ColorButton( bg );
	Layouter( new QHBoxLayout )
		.labeledWidget( tr("&Background color"), bgColor_ )
		.finishTo( mainLayout, 1,0 );

	withPaper_ = new QCheckBox( tr("Paint with &paper") );
	mainLayout->addWidget( withPaper_, 1,2 );

        gamma_ = new QDoubleSpinBox( this );
        gamma_->setRange( 0, 10 );
        gamma_->setSingleStep( 0.1 );
        Layouter( new QHBoxLayout )
                .labeledWidget( tr("&Gamma_ value"), gamma_ )
		.finishTo( mainLayout, 2,0 );

        quality_ = new QSlider( Qt::Horizontal, this );
        quality_->setRange( 0, 100 );
	Layouter( new QHBoxLayout )
                .labeledWidget( tr("&Quality_"), quality_ )
                .finishTo( mainLayout, 2,2 );
        
        connect( xres_,       SIGNAL( valueChanged(int) ),    this, SLOT( noticeChange() ) );
        connect( yres_,       SIGNAL( valueChanged(int) ),    this, SLOT( noticeChange() ) );
        connect( scale_,      SIGNAL( valueChanged(double) ), this, SLOT( noticeChange() ) );
        connect( keepAspect_, SIGNAL( toggled(bool) ),        this, SLOT( noticeChange() ) );
        connect( bgColor_,    SIGNAL( resourceChanged() ),    this, SLOT( noticeChange() ) );
	connect( withPaper_,  SIGNAL( toggled(bool) ),        this, SLOT( noticeChange() ) );
        connect( gamma_,      SIGNAL( valueChanged(double) ), this, SLOT( noticeChange() ) );
        connect( quality_,    SIGNAL( valueChanged(int) ),    this, SLOT( noticeChange() ) );
	
        takeLayout( mainLayout );
}


void PixoutDialog::updateWidgets()
{
	if ( sender() == withPaper_ ) {
		bool wp = withPaper_->isChecked();
		calcFigSize( wp );

		double s = scale_->value() / 100;
		xres_->setValue( qRound( s * figSize_.width() ) );
		yres_->setValue( qRound( s * figSize_.height() ) );
	}
	
		
	bool keepAR = keepAspect_->isChecked();;
	
	if ( sender() == keepAspect_ ) {
		scale_->setEnabled( keepAR );
		yres_->setValue( qRound( (double)xres_->value() * aspectRatio_ ) );
		scale_->setValue( (double)xres_->value() / figSize_.width() * 100 );
	}
	
	if ( keepAR ) {
		if ( sender() == xres_ ) {
			double y = (double)xres_->value() * aspectRatio_;
			if ( y > 1 ) {
				yres_->setValue( qRound(y) );
				scale_->setValue( qRound(100 * y/figSize_.height()) );
			} else 
				xres_->setValue( qRound( (double)yres_->value() / aspectRatio_) );
		} else if ( sender() == yres_ ) {
			double x = yres_->value() / aspectRatio_;
			if ( x > 1 ) {
				xres_->setValue( qRound(x) );
				scale_->setValue( qRound(100 * x/figSize_.width()) );
			} else
				yres_->setValue( qRound( (double)xres_->value() * aspectRatio_) );
		} else if ( sender() == scale_ ) {
			double s = scale_->value() / 100;
			double x = s * figSize_.width();
			double y = s * figSize_.height();

			if ( x > 1 && y > 1 ) {
				xres_->setValue( qRound(x) );
				yres_->setValue( qRound(y) );
			} else
				scale_->setValue( (double)xres_->value()/figSize_.width() * 100 );
		}
	}    
}

void PixoutDialog::applyChanges()
{
	filter_->setXres( xres_->value() );
	filter_->setYres( yres_->value() );
	filter_->setBackground( bgColor_->getResource() );
	filter_->setGamma( gamma_->value() );
	filter_->setQuality( quality_->value() );
	filter_->setWithPaper( withPaper_->isChecked() );    
}

void PixoutDialog::commitChanges()
{
        updateWidgets();
	applyChanges();
}


void PixoutDialog::setupInitialValues()
{
	calcFigSize( false );
	
        xres_->setValue( qRound( figSize_.width() ) );
        yres_->setValue( qRound( figSize_.height() ) );
        scale_->setValue( 100.0 );
	scale_->setEnabled( true );
        keepAspect_->setChecked( true );

	QColor bg( Qt::white );
        bg.setAlpha( 0 );
	bgColor_->setResource( bg );
	withPaper_->setChecked( false );
	
        gamma_->setValue( 1 );
        quality_->setValue( 100 );

	applyChanges();
}

void PixoutDialog::calcFigSize( bool withPaper )
{
        const Figure* fig = filter_->figure_;

	if ( withPaper )
		figSize_ = fig->paper().size();
	else
		figSize_ = fig->boundingRect().size();
	
        aspectRatio_ = figSize_.height() / figSize_.width();	
}
