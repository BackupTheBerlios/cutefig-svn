 
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

#include "penwidget.h"
#include "pen.h"
#include "stylecombobox.h"
#include "flagbuttongroup.h"

#include <QLayout>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QRadioButton>

PenWidget::PenWidget( const QString& title, QWidget* parent )
        : QGroupBox( title, parent )
{
        QGridLayout* lineLayout = new QGridLayout( this );

        lineWidth = new QDoubleSpinBox( this );
        lineWidth->setRange( 1.0, 10.0 );
        lineWidth->setSingleStep( 0.5 );

        QLabel* lwl = new QLabel( tr("&Width"), this );
        lwl->setBuddy( lineWidth );
        lineLayout->addWidget( lwl, 0,0 );
        lineLayout->addWidget( lineWidth, 0,1 );       
   
        lineDashes = new StyleComboBox( this );
        QLabel* lsl = new QLabel( tr("&Style"), this );
        lsl->setBuddy( lineDashes );
        lineLayout->addWidget( lsl, 1,0 );
        lineLayout->addWidget( lineDashes, 1,1 );

        lineLayout->addItem( new QSpacerItem( 10, 0 ), 0,2 );


        QGroupBox* capGroup = new QGroupBox( tr("Ca&p style"), lineLayout->widget() );
        QHBoxLayout* capLayout = new QHBoxLayout( capGroup );

        capStyle = new FlagButtonGroup( capGroup );

        QRadioButton* capFlat = new QRadioButton( capGroup );
        QRadioButton* capRound = new QRadioButton( capGroup );
        QRadioButton* capSquare = new QRadioButton( capGroup );

        capFlat->setIcon( QIcon(":images/cap_flat") );
        capRound->setIcon( QIcon(":images/cap_round") );
        capSquare->setIcon( QIcon(":images/cap_square") );

        capStyle->addButton( capFlat, int( Qt::FlatCap ) );
        capStyle->addButton( capRound, int( Qt::RoundCap ) );
        capStyle->addButton( capSquare, int( Qt::SquareCap ) );

        capLayout->addWidget( capFlat );
        capLayout->addWidget( capRound );
        capLayout->addWidget( capSquare );

        lineLayout->addWidget( capGroup, 0,3 );


        QGroupBox* joinGroup = new QGroupBox( tr("&Join style") );
        QHBoxLayout* joinLayout = new QHBoxLayout( joinGroup );

        joinStyle = new FlagButtonGroup( joinGroup );

        QRadioButton* joinMiter = new QRadioButton( joinGroup );
        QRadioButton* joinBevel = new QRadioButton( joinGroup );
        QRadioButton* joinRound = new QRadioButton( joinGroup );

        joinMiter->setIcon( QIcon(":images/join_miter") );
        joinBevel->setIcon( QIcon(":images/join_bevel") );
        joinRound->setIcon( QIcon(":images/join_round") );

        joinStyle->addButton( joinMiter, int( Qt::MiterJoin ) );
        joinStyle->addButton( joinBevel, int( Qt::BevelJoin ) );
        joinStyle->addButton( joinRound, int( Qt::RoundJoin ) );

        joinLayout->addWidget( joinMiter );
        joinLayout->addWidget( joinBevel );
        joinLayout->addWidget( joinRound );

        lineLayout->addWidget( joinGroup, 1,3 );
        
        connect( lineWidth, SIGNAL( valueChanged(double) ), this, SLOT( changeWidth(double) ) );
        connect( lineDashes, SIGNAL( highlightedDash(const ResourceKey&) ),
                 this, SLOT( changeDashes(const ResourceKey&) ) );
        connect( capStyle, SIGNAL( stateChanged(int) ), this, SLOT( changeCap(int) ) );
        connect( joinStyle, SIGNAL( stateChanged(int) ), this, SLOT( changeJoin(int) ) );
}

void PenWidget::setPen( Pen* pen )
{
        pen_ = pen;
        lineWidth->setValue( pen->width() );
        lineDashes->setCurrentIndex( pen->dashesKey() );
        capStyle->setState( int( pen->capStyle() ) );
        joinStyle->setState( int( pen->joinStyle() ) );
}

void PenWidget::changeWidth( double width )
{
        pen_->setWidth( width );
        emit penChanged();
}

void PenWidget::changeDashes( const ResourceKey& key )
{
        pen_->setDashes( key );
        emit penChanged();
}

void PenWidget::changeCap( int key )
{
        pen_->setCapStyle( Qt::PenCapStyle( key ) );
        emit penChanged();
}

void PenWidget::changeJoin( int key )
{
        pen_->setJoinStyle( Qt::PenJoinStyle( key ) );
        emit penChanged();
}

