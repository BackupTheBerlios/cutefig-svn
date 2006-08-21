 
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

#include <QLayout>

#include <QDebug>


PropDialog::PropDialog( Figure* f )
        : figure_( f ),
          oldMetaData_( figure_->metaData() )
{
        length_ = new ResourceComboBox<Length>( this );
        paper_ = new ResourceComboBox<Paper>( this );

        connect( length_, SIGNAL(valueChanged(int)), this, SLOT(noticeChange()) );
        connect( paper_, SIGNAL(valueChanged(int)), this, SLOT(noticeChange()) );
        
        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("Unit length"), length_ )
                .stretch()
                .labeledWidget( tr("Paper format"), paper_ )
                .finishTo( dialogLayout(), 0 );

        updateValues();
}

void PropDialog::doReset()
{
        figure_->setMetaData( oldMetaData_ );
        updateValues();
}

void PropDialog::updateValues()
{
        length_->setCurrentKey( figure_->unitKey() );
        paper_->setCurrentKey( figure_->paperKey() );
}

void PropDialog::commitChanges( QObject* )
{
        figure_->setUnit( length_->currentKey() );
        figure_->setPaper( paper_->currentKey() );
}
