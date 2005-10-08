 
/*****************************************************************************
**
**  @version $Id: colorbutton.h 16 2005-06-26 22:00:14Z joh $
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

#include "strokewidget.h"

#include "stroke.h"
#include "flagbuttongroup.h"
#include "colorbutton.h"
#include "gradientbutton.h"

#include <QLayout>
#include <QRadioButton>


#include <QDebug>

StrokeWidget::StrokeWidget( const QString& title, QWidget* parent )
        : QGroupBox( title, parent )
{        
        colorButton_ = new ColorButton( Qt::gray, this );
        gradientButton_ = new GradientButton( Gradient(), this );

        QRadioButton* nostrokeRB = new QRadioButton( tr("None"), this );
        QRadioButton* colorRB    = new QRadioButton( tr("Color"), this );
        QRadioButton* gradientRB = new QRadioButton( tr("Gradient"), this );        

        strokeType_ = new FlagButtonGroup( this );

        strokeType_->addButton( nostrokeRB, int( Stroke::sNone ) );
        strokeType_->addButton( colorRB,    int( Stroke::sColor ) );
        strokeType_->addButton( gradientRB, int( Stroke::sGradient ) );

        QGridLayout* layout = new QGridLayout( this );

        layout->addWidget( nostrokeRB, 0,0 );

        layout->addWidget( colorRB, 1,0 );
        layout->addWidget( colorButton_, 1,1 );

        layout->addWidget( gradientRB, 2,0 );
        layout->addWidget( gradientButton_, 2,1 );
        
        connect( colorRB,    SIGNAL( toggled(bool) ), colorButton_,    SLOT( setEnabled(bool) ) );
        connect( gradientRB, SIGNAL( toggled(bool) ), gradientButton_, SLOT( setEnabled(bool) ) );

        connect( colorButton_, SIGNAL( colorChanged(QColor) ), this, SLOT( setColor() ) );
        connect( gradientButton_, SIGNAL(gradientChanged(Gradient*)), this, SLOT(setGradient()) );
        connect( strokeType_, SIGNAL( stateChanged(int) ), this, SLOT( changeType(int) ) );
}

void StrokeWidget::setStroke( Stroke* stroke )
{
        stroke_ = stroke;
        colorButton_->setEnabled( stroke->type() == Stroke::sColor );
        gradientButton_->setEnabled( stroke->type() == Stroke::sGradient );
        strokeType_->setState( int( stroke->type() ) );

        switch( stroke->type() ) {
            case Stroke::sColor:
                    colorButton_->setColor( stroke->color() ); break;
            case Stroke::sGradient:
                    gradientButton_->setGradient( stroke->gradient() ); break;
            default:
                    break;
        }
}

void StrokeWidget::changeType( int type )
{
        switch( Stroke::StrokeType( type ) ) {
            case Stroke::sNone:
                    stroke_->setNone(); break;
            case Stroke::sColor:
                    stroke_->setColor( colorButton_->getColor() ); break;
            case Stroke::sGradient:
                    stroke_->setGradient( gradientButton_->getGradient() ); break;
            default:
                    break;
        }

        emit strokeChanged();
}

void StrokeWidget::setColor()
{
        stroke_->setColor( colorButton_->getColor() );
        emit strokeChanged();
}

void StrokeWidget::setGradient()
{
        stroke_->setGradient( gradientButton_->getGradient() );
        emit strokeChanged();
}
