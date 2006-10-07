 
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
#include "objectguihandler.h"
#include <QTabWidget>

class EditdialogAction;
class DrawObject;

class QGroupBox;

class QSpinBox;
class QTextEdit;

class StrokeWidget;
class ArrowWidget;
class Arrow;
class PenWidget;

#include <QDebug>

class ObjectDialog : public EditDialog
{
        Q_OBJECT
public:
        ObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent=0 );

	virtual void accept();
	
protected:
	QTabWidget* tabWidget() { return tabWidget_; }
        DrawObject* drawObject() { return drawObject_; }
        PenWidget*  penWidget() { return penWidget_; }

private:
        virtual void doReset();
        virtual void commitChanges();
        virtual void commitChangesPrivate() {}
        virtual void setupInitialValues();
        virtual void setupInitialValuesPrivate() {}
        
        virtual void castDrawObject() {};

        void setDrawObject( DrawObject* o );
        
        EditdialogAction* action_;

        DrawObject* drawObject_;
        QTabWidget* tabWidget_;

        PenWidget* penWidget_;
        StrokeWidget* lineStroke_;
        StrokeWidget* fillStroke_;
        ArrowWidget* arrows_;
        QTextEdit* comment_;
        QSpinBox* depth_;
};

#endif
