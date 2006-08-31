 
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

#include "statuswidgets.h"
#include "figure.h"
#include "actionstatus.h"

#include <QTextStream>
#include <QStatusBar>

#include <QDebug>



CoordWidget::CoordWidget( const Figure& fig, QWidget* parent )
        : QLabel( parent ),
          figure_( fig )
{
        setFrameStyle( QFrame::Panel );
        setMinimumWidth( QFontMetrics( QFont() ).width('0') * 15 );
}

void CoordWidget::setCoords( const QPoint& p )
{
        pos_ = p;
        updateContents();
}

void CoordWidget::setIndicating( bool ind )
{
        indicating_ = ind;
        updateContents();
}

void CoordWidget::updateContents()
{
        if ( !indicating_ ) {
                clear();
                return;
        }
        
        QString s;
        
        double x = double(pos_.x())/figure_.unit();
        double y = double(pos_.y())/figure_.unit();

        QTextStream ts( &s );
        ts.setRealNumberPrecision( 2 );
        ts.setRealNumberNotation( QTextStream::FixedNotation );

        ts << x << " : " << y << " " << figure_.unitKey().keyString();

        setText( s );
}



ActionStatusIndicator::ActionStatusIndicator( QStatusBar* parent )
        : QObject( parent )
{
        left_ = new QLabel( leftString() );
        left_->setMinimumWidth( QFontMetrics( QFont() ).width('0') * 10 );
        middle_ = new QLabel( middleString() );
        middle_->setMinimumWidth( QFontMetrics( QFont() ).width('0') * 10 );
        right_ = new QLabel( rightString() );
        right_->setMinimumWidth( QFontMetrics( QFont() ).width('0') * 10 );
        help_ = new QLabel();
        help_->setMinimumWidth( QFontMetrics( QFont() ).width('0') * 5 );
        status_ = new QLabel();
        status_->setMinimumWidth( QFontMetrics( QFont() ).width('0') * 5 );

        ctrl_ = new QLabel( tr("C") );
        ctrl_->setDisabled( true );
        alt_ = new QLabel( tr("A") );
        alt_->setDisabled( true );
        shift_ = new QLabel( tr("S") );
        shift_->setDisabled( true );

        parent->addPermanentWidget( left_ );
        parent->addPermanentWidget( middle_ );
        parent->addPermanentWidget( right_ );
        parent->addPermanentWidget( help_ );
        parent->addPermanentWidget( status_ );
        parent->addPermanentWidget( ctrl_ );
        parent->addPermanentWidget( alt_ );
        parent->addPermanentWidget( shift_ );

        disableAndClear();
}

void ActionStatusIndicator::disableAndClear()
{
        left_->setText( leftString() );
        right_->setText( rightString() );
        middle_->setText( middleString() );
        help_->clear();
        status_->clear();

        ctrl_->setDisabled( true );
        alt_->setDisabled( true );
        shift_->setDisabled( true );

        left_->setDisabled( true );
        right_->setDisabled( true );
        middle_->setDisabled( true );
        help_->setDisabled( true );
        status_->setDisabled( true );    
}

void ActionStatusIndicator::enable()
{
        left_->setEnabled( true );
        right_->setEnabled( true );
        middle_->setEnabled( true );
        help_->setEnabled( true );
        status_->setEnabled( true );
}

void ActionStatusIndicator::beActive( bool a )
{
        if ( a )
                enable();
        else
                disableAndClear();
}

void ActionStatusIndicator::showStatus( const ActionStatus& st )
{
        const ActionStatus::Information& info = st.information();
        
        left_->setText( leftString() + " " + info.left() );
        right_->setText( rightString() + " " + info.right() );
        middle_->setText( middleString() + " " + info.mid() );
        help_->setText( info.help() );
        status_->setText( st.status() );

        ctrl_->setEnabled( st.modifiers() & Qt::ControlModifier );
        alt_->setEnabled( st.modifiers() & Qt::AltModifier );
        shift_->setEnabled( st.modifiers() & Qt::ShiftModifier );
}


QString ActionStatusIndicator::leftString()
{
        return tr("L: ");
}

QString ActionStatusIndicator::middleString()
{
        return tr("M: ");
}

QString ActionStatusIndicator::rightString()
{
        return tr("R: ");
}


