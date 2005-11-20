 
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

#ifndef objectdialog_h
#define objectdialog_h

#include "editdialog.h"

#include <QTabWidget>

class EditdialogAction;
class DrawObject;

class QGroupBox;

class QSpinBox;
class QTextEdit;

class LineShowWidget;
class StrokeWidget;
class PenWidget;

#include <QDebug>

class ObjectDialog : public EditDialog
{
        Q_OBJECT
public:
        ObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent=0 );
        ~ObjectDialog() {};
        
        DrawObject* backupObject() const { return backup_; };
        DrawObject* drawObject() const { return drawObject_; };

public slots:
        virtual void reset();
        virtual void accept();

        void showValue( int v ) {qDebug() << v;}
        
protected:
        DrawObject *drawObject_, *backup_;
        QTabWidget* tabWidget;
        
        void setUpAll();
        
        virtual void setUpConnections();
        virtual void setUpConnectionsPrivate() {}
        
        virtual void setDefaultValues();
        virtual void setDefaultValuesPrivate() {}
        
        virtual void setUpWidgetsPrivate() {}
        
        virtual void castDrawObject() =0;
        
        EditdialogAction* action_;
        
 
//private:
        void setUpGeneral();   
        void setDrawObject( DrawObject* o );
        
//        QGroupBox* setUpLineGroup( QWidget* page );
//        QGroupBox* setUpFillGroup( QWidget* page );

        PenWidget* penWidget;
        StrokeWidget* lineStroke;
        StrokeWidget* fillStroke;
        LineShowWidget* lineShow;
        QTextEdit* comment;
        QSpinBox* depth;
};

#endif
