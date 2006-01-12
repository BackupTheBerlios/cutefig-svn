 
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

#ifndef penwidget_h
#define penwidget_h

#include "reslib.h"

#include <QGroupBox>

class Pen;

class QDoubleSpinBox;
template<typename Resource> class ResourceComboBox;
class FlagButtonGroup;

class PenWidget : public QGroupBox
{
        Q_OBJECT
public:
        PenWidget( const QString& title, QWidget* parent = 0 );
        ~PenWidget() {}

        void setPen( Pen* pen );
        
signals:
        void penChanged();

private slots:
        void changeWidth( double width );
        void changeDashes( const ResourceKey& key );
        void changeCap( int key );
        void changeJoin( int key );
        
private:
        Pen* pen_;

        QDoubleSpinBox* lineWidth;
        ResourceComboBox<Dashes>* lineDashes;
        FlagButtonGroup *capStyle, *joinStyle;
};


#endif
