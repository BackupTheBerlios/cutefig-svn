 
/*****************************************************************************
**
**  @version $Id: ellipsedialog.h 117 2006-08-21 13:54:16Z joh $
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

#ifndef rectangloiddialog_h
#define rectangloiddialog_h

#include "objectdialog.h"

class QSlider;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QButtonGroup;
class QAbstractButton;
class QVBoxLayout;

class RectangloidDialog : public ObjectDialog
{
        Q_OBJECT
public:
        RectangloidDialog( DrawObject* o, EditdialogAction* a, QWidget* parent=0 );

protected:
        QVBoxLayout* pageLayout() const { return pageLayout_; }

        QSlider* angleSlider() { return angleSlider_; }
        
private:
        QSlider* angleSlider_;
        QSpinBox* angleSpin_;

        QVBoxLayout* pageLayout_;
};


class Rectangle;


class RectangleDialog : public RectangloidDialog
{
        Q_OBJECT
public:
        RectangleDialog( DrawObject* o, EditdialogAction* a, QWidget* parent = 0 );

private:
        Rectangle* rectangle_;

        virtual void commitChangesPrivate();
        virtual void setupInitialValuesPrivate();

        virtual void castDrawObject();

        void setRoundedCornersEnabled( bool e );
        
        QCheckBox* roundedCorners_;
        QDoubleSpinBox* xCornerRadius_;
        QDoubleSpinBox* yCornerRadius_;
        QAbstractButton* equalCornerRadii_;
};



class Ellipse;

class EllipseDialog : public RectangloidDialog
{
        Q_OBJECT
public:
        EllipseDialog( DrawObject* o, EditdialogAction* a, QWidget* parent = 0 );

private:
        Ellipse* ellipse_;
        
        virtual void commitChangesPrivate();
        virtual void setupInitialValuesPrivate();

        virtual void castDrawObject();
        
        QButtonGroup* subType_;
        QButtonGroup* definition_;
};


#endif
