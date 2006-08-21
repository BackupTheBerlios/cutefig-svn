 
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

class ResourceEditor : public QObject
{
        Q_OBJECT
public:
        ResourceEditor( EditDialog* dlg, QVBoxLayout* layout, QObject* parent = 0 )
                : QObject( parent ),
                  dialog_( dlg ),
                  layout_( layout ) 
        {
        }

        virtual void updateData() = 0;

signals:
        void changeHappened();

protected:
        EditDialog* dialog_;
        QVBoxLayout* layout_;
};


template<class Resource>
class ResourceDialog : public EditDialog
{
public:        
        static int execute( Resource& editee, QWidget* parent = 0 ) 
        {
                ResourceDialog<Resource> dlg( editee, parent );
                dlg.init();
                return dlg.exec();
        }

        static Resource editData( const Resource& initial, bool* ok = 0, QWidget* parent = 0 )
        {
                Resource res = initial;

                bool accepted = ( execute( res, parent ) == QDialog::Accepted );
                if ( ok )
                        *ok = accepted;

                return res;
        }       
                
        virtual void doReset()
        {
                editee_ = oldData_;
                editor_->updateData();
        }

        
protected:
        ResourceDialog<Resource>( Resource& editee, QWidget* parent = 0 )
                : EditDialog( parent ),
                  editee_( editee ),
                  oldData_( editee )
        {}
                  
        ~ResourceDialog<Resource>() {}

        void init() 
        {
                editor_ = createEditor();
                connect( editor_, SIGNAL( changeHappened() ), this, SLOT( noticeChange() ) );
        }

        ResourceEditor* createEditor();
        
        Resource& editee_;
        Resource oldData_;

        ResourceEditor* editor_;
};


// #include "colordialog.h"

// template<> class ResourceDialog<QColor> 
// {
// public:
//         static int execute( QColor& editee, QWidget* parent = 0 ) 
//         {
//                 bool ok;
//                 QColor c = editData( editee, &ok, parent );
//                 if ( ok )
//                         editee = c;

//                 return ok;
//         }

//         static QColor editData( const QColor& initial, bool* ok = 0,
//                                 QWidget* parent = 0 ) 
//         {
//                 return ColorDialog::getColor( initial, ok, parent );
//         }

// };


#endif
