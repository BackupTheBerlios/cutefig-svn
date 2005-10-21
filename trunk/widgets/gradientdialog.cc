 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#include "gradientdialog.h"
#include "gradientwidget.h"
#include "flagbuttongroup.h"
#include "strokeiconengines.h"

#include "reslib.h"
#include "strokelib.h"

#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
//#include <QListWidget>
#include <QIcon>

#include <QDebug>

GradientDialog::GradientDialog( const Gradient& gradient, QWidget* parent )
        : EditDialog( parent ),
          gradient_( gradient ),
          oldGradient_( gradient_ )
//          radius_( 0.2 )
{        
        QHBoxLayout* mainLayout = new QHBoxLayout();
        
        gradWidget_ = new GradientWidget( &gradient_, this );
        gradWidget_->setMinimumSize( QSize( 300,200 ) );
        mainLayout->addWidget( gradWidget_ );

        QGroupBox* typeGroupBox = new QGroupBox( tr("Gradient &type"), this );
        mainLayout->addWidget( typeGroupBox );

        QVBoxLayout* typeBoxLayout = new QVBoxLayout( typeGroupBox );

        gradType_ = new FlagButtonGroup( typeGroupBox );
        
        QRadioButton* linear = new QRadioButton( tr("&Linear"), typeGroupBox );
        QRadioButton* radial = new QRadioButton( tr("Ra&dial"), typeGroupBox );

        gradType_->addButton( linear, int( Gradient::Linear ) );
        gradType_->addButton( radial, int( Gradient::Radial ) );
        
        typeBoxLayout->addWidget( linear );
        typeBoxLayout->addWidget( radial );

        switch ( gradient_.type() ) {
            case Gradient::Linear:
                    linear->setChecked( true ); break;
            case Gradient::Radial:
                    radial->setChecked( true ); break;
            default: break;
        }
        
        connect( gradType_, SIGNAL( stateChanged(int) ), this, SLOT( typeChanged(int) ) );

//         gradListWgt_ = new QListWidget( this );
//         fillGradList();
//         connect( gradListWgt_, SIGNAL( itemActivated(QListWidgetItem*) ),
//                  this, SLOT( changeGradientFromList(QListWidgetItem*) ) );
//         mainLayout->addWidget( gradListWgt_ );
        
        dialogLayout_->insertLayout( 0, mainLayout );
}

void GradientDialog::reset()
{
        gradient_ = oldGradient_;
        gradWidget_->setGradient( &gradient_ );
}

void GradientDialog::typeChanged( int type )
{
        gradient_.setType( Gradient::Type( type ) );
        gradWidget_->setGradient( &gradient_ );
}

// void GradientDialog::fillGradList()
// {
//         StrokeLib& sl = StrokeLib::instance();

//         foreach( ResourceKey key, sl.keys() ) {
//                 qDebug() << key.keyString() << "builtIn" << key.isBuiltIn() << "valid" << key.isValid();
//                 const Stroke& s = sl[key];
//                 if ( s.type() == Stroke::sGradient ) {
//                         QListWidgetItem* wi = new QListWidgetItem( key.keyString(), gradListWgt_ );
//                         wi->setIcon( QIcon( new GradientIconEngine( s.gradient() ) ) );
//                         gradHash_[wi] = key;
//                 }
//         }
// }

// void GradientDialog::changeGradientFromList( QListWidgetItem* wi )
// {
//         const ResourceKey& key = gradHash_[wi];
        
//         gradient_ = StrokeLib::instance()[key].gradient();
        

//         gradType_->setState( gradient_.type() );

//         gradWidget_->setDisabled( key.isBuiltIn() );
//         gradWidget_->setGradient( &gradient_ );
// }

Gradient GradientDialog::getGradient( const Gradient& initial, bool *ok, QWidget* parent )
{
        GradientDialog dlg( initial, parent );

        bool accepted = ( dlg.exec() == QDialog::Accepted );
        if ( ok )
                *ok = accepted;

        Gradient result;
        if ( accepted )
                result = dlg.gradient();

        return result;
}
