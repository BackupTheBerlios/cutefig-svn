 
/*****************************************************************************
**
**  $Id: gradienteditor.h 48 2005-11-07 10:05:25Z joh $
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

#include "arrowwidget.h"
#include "resourcecombobox.h"
#include "layouter.h"

#include <QLayout>
#include <QDoubleSpinBox>

QDoubleSpinBox* makeDoubleSpinBox( QWidget* w )
{
        QDoubleSpinBox* dsb = new QDoubleSpinBox( w );
        dsb->setMinimum( 0.5 );
        dsb->setMaximum( 20.0 );
        dsb->setDecimals( 2 );
        dsb->setSingleStep( 0.25 );

        return dsb;
}


ArrowWidget::ArrowWidget( const QString& title, QWidget* parent )
        : QGroupBox( title, parent ),
          startWidth_( makeDoubleSpinBox( this ) ),
          startLength_( makeDoubleSpinBox( this ) ),
          endWidth_( makeDoubleSpinBox( this ) ),
          endLength_( makeDoubleSpinBox( this ) ),
          startBox_( new ResourceComboBox<ArrowPainter*>( this ) ),
          endBox_( new ResourceComboBox<ArrowPainter*>( this ) )
{
        QVBoxLayout* l = new QVBoxLayout( this );
        
        Layouter( new QHBoxLayout() ).labeledWidget( tr("&Start arrow"), startBox_ ).finishTo( l );
        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("Width"), startWidth_ ).stretch()
                .labeledWidget( tr("Length"), startLength_ )
                .finishTo( l );
 
        Layouter( new QHBoxLayout() ).labeledWidget( tr("&End arrow"), endBox_ ).finishTo( l );
        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("Width"), endWidth_ ).stretch()
                .labeledWidget( tr("Length"), endLength_ )
                .finishTo( l );

        connect( startBox_, SIGNAL( activated( const ResourceKey& ) ),
                 this, SLOT( startPainterChanged( const ResourceKey& ) ) );
        connect( endBox_, SIGNAL( activated( const ResourceKey& ) ),
                 this, SLOT( endPainterChanged( const ResourceKey& ) ) );

        connect( startWidth_, SIGNAL( valueChanged(double) ),
                 this, SLOT( startWidthChanged(double) ) );
        connect( startLength_, SIGNAL( valueChanged(double) ),
                 this, SLOT( startLengthChanged(double) ) );
        connect( endWidth_, SIGNAL( valueChanged(double) ),
                 this, SLOT( endWidthChanged(double) ) );
        connect( endLength_, SIGNAL( valueChanged(double) ),
                 this, SLOT( endLengthChanged(double) ) );
}

void ArrowWidget::setArrows( const Arrow& start, const Arrow& end )
{
        start_ = start;
        end_ = end;
        
        startBox_->setCurrentKey( start.key() );
        startWidth_->setValue( start.width() );
        startLength_->setValue( start.length() );
        
        endBox_->setCurrentKey( end.key() );
        endWidth_->setValue( end.width() );
        endLength_->setValue( end.length() );
}

void ArrowWidget::startPainterChanged( const ResourceKey& key )
{
        start_.setArrowPainter( key );
        emit startArrowChanged( start_ );
}

void ArrowWidget::endPainterChanged( const ResourceKey& key )
{
        end_.setArrowPainter( key );
        emit endArrowChanged( end_ );
}

void ArrowWidget::startWidthChanged( double v )
{
        start_.setWidth( v );
        emit startArrowChanged( start_ );
}

void ArrowWidget::startLengthChanged( double v )
{
        start_.setLength( v );
        emit startArrowChanged( start_ );
}

void ArrowWidget::endWidthChanged( double v )
{
        end_.setWidth( v );
        emit endArrowChanged( end_ );
}

void ArrowWidget::endLengthChanged( double v )
{
        end_.setLength( v );
        emit endArrowChanged( end_ );
}

template<>
void ResourceComboBox<ArrowPainter*>::init() 
{
        setIconSize( QSize( iconSize().width()*3, iconSize().height() ) );
        insertItem( 0, QIcon( new ResourceIconEngine<ArrowPainter*>( 0 ) ), QString() );
        keys_.prepend( ResourceKey() );
}


