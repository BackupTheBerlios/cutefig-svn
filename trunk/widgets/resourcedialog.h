 
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

#ifndef resourcedialog_h
#define resourcedialog_h

#include "editdialog.h"

class ResourceEditor : public QWidget
{
        Q_OBJECT
public:
        ResourceEditor( QWidget* parent = 0 )
                : QWidget( parent )
        {
        }

        virtual void updateData() = 0;

signals:
        void changeHappened();

protected:
        EditDialog* dialog_;
};


template<class Resource>
class ResourceDialog : public EditDialog
{
public:        
        static int execute( Resource& editee, QWidget* parent = 0 );
        
        static Resource editData( const Resource& initial, bool* ok = 0, QWidget* parent = 0 );

        virtual void doReset();
        virtual void commitChanges() {}
        virtual void setupInitialValues() {}
        
protected:
        ResourceDialog<Resource>( Resource& editee, QWidget* parent = 0 );

        ResourceEditor* createEditor();
        
        Resource& editee_;
        Resource oldData_;

        ResourceEditor* editor_;
};

template<typename Resource>
ResourceDialog<Resource>::ResourceDialog( Resource& editee, QWidget* parent )
        : EditDialog( parent ),
          editee_( editee ),
          oldData_( editee )
{
        editor_ = createEditor();
        takeWidget( editor_ );
        connect( editor_, SIGNAL( changeHappened() ), this, SLOT( noticeChange() ) );
}

        

template<typename Resource>
int ResourceDialog<Resource>::execute( Resource& editee, QWidget* parent ) 
{
        ResourceDialog<Resource> dlg( editee, parent );
        return dlg.exec();
}


template<typename Resource>
Resource ResourceDialog<Resource>::editData( const Resource& initial, bool* ok,
                                             QWidget* parent )
{
        Resource res = initial;

        bool accepted = ( execute( res, parent ) == QDialog::Accepted );
        if ( ok )
                *ok = accepted;

        return res;
}


template<typename Resource>
void ResourceDialog<Resource>::doReset()
{
        editee_ = oldData_;
}



#endif
