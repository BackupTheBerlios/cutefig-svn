 
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

#ifndef editdialog_h
#define editdialog_h

#include <QDialog>

class QVBoxLayout;

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
