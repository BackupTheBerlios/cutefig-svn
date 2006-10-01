 
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

#include "editdialog.h"

#include <QLayout>
#include <QPushButton>
#include <QTimer>

#include <QDebug>

/*! Sets up the standard widgets and calls init() by the delayed
 *  initialisation idiom. This way init() is called by the event loop,
 *  so it is guaranteed that the whole concrete EditDialog object is
 *  properly constructed by the time init() and thus
 *  setupInitialValues() are called.
 */
EditDialog::EditDialog( QWidget* parent )
        : QDialog( parent ),
          dialogLayout_( new QVBoxLayout( this ) ),
          changed_( false ),
          blindForChanges_( true )
{
        QHBoxLayout* bottomLayout = new QHBoxLayout();

        QPushButton* ok = new QPushButton( tr("&Ok"), this );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        ok->setAutoDefault( true );

        QPushButton* cancel = new QPushButton( tr("Cancel"), this );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

        reset_ = new QPushButton( tr("&Reset"), this );
        connect( reset_, SIGNAL( clicked() ), this, SLOT( resetChanges() ) );
        reset_->setEnabled( false );
        
        bottomLayout->addWidget( reset_ );
        bottomLayout->addStretch();
        bottomLayout->addWidget( ok );
        bottomLayout->addWidget( cancel );

        dialogLayout_->setSpacing( 6 );
        dialogLayout_->insertLayout( 1, bottomLayout );

        QTimer::singleShot( 0, this, SLOT( initialize() ) );
}

void EditDialog::takeLayout( QLayout* layout, int stretch )
{
        dialogLayout_->insertLayout( 0, layout, stretch );
}

void EditDialog::takeWidget( QWidget* widget, int stretch )
{
        dialogLayout_->insertWidget( 0, widget, stretch );
}

void EditDialog::resetChanges()
{
        blindForChanges_ = true;
        doReset();
        initialize();
        blindForChanges_ = false;
        
        changed_ = false;
        reset_->setEnabled( false );
        emit changeHappened();
}

void EditDialog::noticeChange()
{
        qDebug() << __PRETTY_FUNCTION__ << blindForChanges_;
        if ( blindForChanges_ )
                return;

        blindForChanges_ = true;
        changed_ = true;
        commitChanges();
        reset_->setEnabled( true );
        emit changeHappened();
        blindForChanges_ = false;
}

void EditDialog::reject()
{
        if ( changed_ )
                resetChanges();
        QDialog::reject();
}

void EditDialog::initialize()
{
        setupInitialValues();
        blindForChanges_ = false;

        qDebug() << __PRETTY_FUNCTION__ << this << blindForChanges_;
}
