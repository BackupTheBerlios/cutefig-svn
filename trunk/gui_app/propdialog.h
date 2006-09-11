 
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

#ifndef propdialog_h
#define propdialog_h

#include "editdialog.h"
#include "figure.h"


template<typename Resource> class ResourceComboBox;
class QLineEdit;
class QTextEdit;
class QCheckBox;


class PropDialog : public EditDialog
{
public:
        PropDialog( Figure* f, QWidget* parent = 0 );
        
private:
        Figure* figure_;

        virtual void doReset();
        virtual void commitChanges( QObject* sender );
        
        void updateValues();

        Figure::MetaData oldMetaData_;

        ResourceComboBox<Length>* length_;
        ResourceComboBox<Paper>* paper_;

	QCheckBox* saveAuthor_;
        QLineEdit* author_;
        QTextEdit* description_;
};



#endif
