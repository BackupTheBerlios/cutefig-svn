 
/*****************************************************************************
**
**  @version $Id: colorbutton.h 16 2005-06-26 22:00:14Z joh $
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

#ifndef strokewidget_h
#define strokewidget_h

#include <QGroupBox>

class ColorButton;
class GradientButton;
class FlagButtonGroup;

class Stroke;

class StrokeWidget : public QGroupBox 
{
        Q_OBJECT
public:
        StrokeWidget( const QString& title, QWidget* parent = 0 );
        ~StrokeWidget() {}

        void setStroke( Stroke* stroke );
        
signals:
        void strokeChanged();

private slots:
        void changeType( int type );
        void setColor();
        void setGradient();
        
private:
        Stroke* stroke_;

        FlagButtonGroup* strokeType_;
        ColorButton* colorButton_;
        GradientButton* gradientButton_;
};

#endif
