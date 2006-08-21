 
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

#ifndef editdialog_h
#define editdialog_h

#include <QDialog>

class QVBoxLayout;

//! Provides a dialog with three standard buttons at the bottom
/** To edit the properties of some object I need a dialog with three
 *  standard buttons at the bottom. These are "Ok" "Cancel" and "Reset"
 *
 *  The buttons "Ok" and "Cancel" are connected to the standart slots
 *  of QDialog which are QDialog::accept() and QDialog::reject(). The
 *  button "Reset" is connected to the pure virtual slot reset() which
 *  is to be implemented by the actual dialog.
 *
 *  The widgets needed by the actual dialog can be put into
 *  dialogLayout_
 */
class EditDialog : public QDialog
{
        Q_OBJECT
public:
        EditDialog( QWidget* parent = 0 );
        ~EditDialog() {}

protected slots:
        virtual void reset() = 0;

protected:
        QVBoxLayout* dialogLayout_;
};

#endif
