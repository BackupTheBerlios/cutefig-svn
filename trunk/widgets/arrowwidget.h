 
/*****************************************************************************
**
**  $Id: gradienteditor.h 48 2005-11-07 10:05:25Z joh $
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

#ifndef arrowwidget_h
#define arrowwidget_h

#include "arrow.h"

#include <QGroupBox>

class QDoubleSpinBox;


template<typename Resource> class ResourceComboBox;

class ArrowWidget : public QGroupBox
{
        Q_OBJECT
public:
        ArrowWidget( const QString& title, QWidget* parent = 0 );
        ~ArrowWidget() {}

        void setArrows( const Arrow& start, const Arrow& end );
        Arrow startArrow() const;
        Arrow endArrow() const;

signals:
        void startArrowChanged( const Arrow& );
        void endArrowChanged( const Arrow& );

private slots:
        void startPainterChanged( const ResourceKey& key );
        void endPainterChanged( const ResourceKey& key );

        void startWidthChanged( double v );
        void startLengthChanged( double v );
        void endWidthChanged( double v );
        void endLengthChanged( double v );

private:
        Arrow start_, end_;

        QDoubleSpinBox* startWidth_;
        QDoubleSpinBox* startLength_;
        QDoubleSpinBox* endWidth_;
        QDoubleSpinBox* endLength_;

        ResourceComboBox<ArrowPainter*>* startBox_;
        ResourceComboBox<ArrowPainter*>* endBox_;
};


#endif
