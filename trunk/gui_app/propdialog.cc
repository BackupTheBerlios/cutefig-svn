 
/*****************************************************************************
**
**  @version $Id: cutefig.cc 114 2006-08-17 18:31:18Z joh $
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
        : figure_( f )
{
        length_ = new ResourceComboBox<Length>( this );
        paper_ = new ResourceComboBox<Paper>( this );

        Layouter( new QHBoxLayout() )
                .labeledWidget( tr("Unit length"), length_ )
                .stretch()
                .labeledWidget( tr("Paper format"), paper_ )
                .finishTo( dialogLayout_, 0 );

        updateValues();
}

void PropDialog::reset()
{
//        figure_->setMetaData( oldMetaData_ );
        updateValues();
}

void PropDialog::updateValues()
{
        const Figure::MetaData& md = figure_->metaData();
        length_->setCurrentKey( md.unit().key() );
        paper_->setCurrentKey( md.paper().key() );
}

void PropDialog::accept()
{
        figure_->setUnit( length_->currentKey() );
        figure_->setPaper( paper_->currentKey() );

        QDialog::accept();
}
