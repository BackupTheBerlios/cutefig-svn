 
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

#include "propdialog.h"
#include "layouter.h"
#include "resourcecombobox.h"

#include <QtGui>

#include <QDebug>


PropDialog::PropDialog( Figure* f, QWidget* parent )
        : EditDialog( parent ),
          figure_( f ),
          oldMetaData_( figure_->metaData() )
{
        QGroupBox* geomGrp = new QGroupBox( tr("&Geometry") );

        length_ = new ResourceComboBox<Length>;
        paper_ = new ResourceComboBox<Paper>;

        QVBoxLayout* gl = new QVBoxLayout;
        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("&Unit length"), length_ )
                .stretch()
                .labeledWidget( tr("&Paper format"), paper_ )
                .finishTo( gl );

        geomGrp->setLayout( gl );
        
        connect( length_, SIGNAL(activated(int)), this, SLOT(noticeChange()) );
        connect( paper_, SIGNAL(activated(int)), this, SLOT(noticeChange()) );
        

        QGroupBox* metaDatGrp = new QGroupBox( tr("&Meta data") );

        author_ = new QLineEdit;
        description_ = new QTextEdit;
        description_->setAcceptRichText( false );

        QVBoxLayout* metaLt = new QVBoxLayout;

        QLabel* authorLab = new QLabel( tr("&Author") );
        authorLab->setBuddy( author_ );
        metaLt->addWidget( authorLab );
        metaLt->addWidget( author_ );

        QLabel* descLab = new QLabel( tr("&Description") );
        descLab->setBuddy( description_ );
        metaLt->addWidget( descLab );
        metaLt->addWidget( description_ );

        metaDatGrp->setLayout( metaLt );

        connect( author_, SIGNAL(textEdited(const QString&)), this, SLOT(noticeChange()) );
        connect( description_, SIGNAL(textChanged()), this, SLOT(noticeChange()) );

        
        QGroupBox* dateGrp = new QGroupBox( tr("Dates") );
        QGridLayout* dateLt = new QGridLayout;

        QLabel* cDatLab = new QLabel( tr("Date of first creation:") );
        QLabel* mDatLab = new QLabel( tr("Date of last modification:") );
        QLabel* cDate = new QLabel( figure_->creationDate().toString( Qt::TextDate ) );
        cDate->setFrameStyle( QFrame::Sunken | QFrame::Panel );
        QLabel* mDate = new QLabel( figure_->modificationDate().toString( Qt::TextDate ) );
        mDate->setFrameStyle( QFrame::Sunken | QFrame::Panel );

        dateLt->addWidget( cDatLab, 0, 0 );
        dateLt->addWidget( cDate,   0, 1 );
        dateLt->addWidget( mDatLab, 1, 0 );
        dateLt->addWidget( mDate,   1, 1 );
        
        dateGrp->setLayout( dateLt );
        

        dialogLayout()->insertWidget( 0, dateGrp );
        dialogLayout()->insertWidget( 0, metaDatGrp );
        dialogLayout()->insertWidget( 0, geomGrp );
        
        updateValues();
}

void PropDialog::doReset()
{
        figure_->setMetaData( oldMetaData_ );
        updateValues();
}

void PropDialog::updateValues()
{
        doNoticeChanges( false );
        qDebug() << "updateValues" << figure_->description();
        length_->setCurrentKey( figure_->unitKey() );
        paper_->setCurrentKey( figure_->paperKey() );
        author_->setText( figure_->author() );
        description_->setPlainText( figure_->description() );
        qDebug() << "updateValues end" << description_->toPlainText();
        doNoticeChanges( true );
}

void PropDialog::commitChanges( QObject* )
{
        qDebug() << "commitchanges" << description_->toPlainText();
        figure_->setUnit( length_->currentKey() );
        figure_->setPaper( paper_->currentKey() );
        figure_->setAuthor( author_->text() );
        figure_->setDescription( description_->toPlainText() );
}




