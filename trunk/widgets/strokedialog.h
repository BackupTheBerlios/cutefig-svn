 
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

#ifndef strokedialog_h
#define strokedialog_h

#include "editdialog.h"
#include "resourcekey.h"
#include "typedefs.h"

#include <QModelIndex>

class QAction;
class QItemSelectionModel;
class StrokeModel;
class StrokeDemo;

class StrokeDialog : public EditDialog
{
        Q_OBJECT
public:
        StrokeDialog( const ResourceKey& initial, const ResourceKeyList& keys = ResourceKeyList(),
                      QWidget* parent=0 );
        ~StrokeDialog() {}

        ResourceKey strokeKey() { return strokeKey_; }

        static ResourceKey getStroke( const ResourceKey& initial, bool* ok = 0,
                                      const ResourceKeyList& keys = ResourceKeyList(),
                                      QWidget* parent = 0 );
        
protected slots:
        virtual void reset();

private slots:
        void strokeChanged( const QModelIndex& index, const QModelIndex& prev );
        void selectionChanged( const QModelIndex& index );
        void copyStroke();
        void deleteStroke();
        void editStroke();

private:
        void setStroke( const ResourceKey& key );
        
        QAction* copyStrokeAction_;
        QAction* deleteStrokeAction_;
        QAction* editStrokeAction_;

        StrokeModel* strokeModel_;
        QItemSelectionModel* selectedStroke_;

        StrokeDemo* strokeDemo_;
        
        ResourceKey oldStrokeKey_, strokeKey_;
        StrokeLib& strokeLib;
        const ResourceKeyList& keys_;
};
        
#endif
