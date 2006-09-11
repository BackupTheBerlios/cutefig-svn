 
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

#ifndef statuswidgets_h
#define statuswidgets_h


#include <QLabel>

class Figure;
class ActionStatus;

class QStatusBar;


//! QLabel that indicates the cursor coordinates in CanvasView in a QStatusBar
class CoordWidget : public QLabel
{
        Q_OBJECT
public:
        CoordWidget( const Figure& fig, QWidget* parent = 0 );
        
public slots:
        void setCoords( const QPoint& p );
        void setIndicating( bool ind );

private:
        const Figure& figure_;
        QPoint pos_;
        bool indicating_;

        void updateContents();
};

//! Sets up and maintains QLabels to display an ActionStatus.
class ActionStatusIndicator : public QObject 
{
        Q_OBJECT
public:
        ActionStatusIndicator( QStatusBar* parent );

public slots:
        void showStatus( const ActionStatus& st );
        
private:
        QLabel* left_;
        QLabel* middle_;
        QLabel* right_;
        QLabel* help_;
        QLabel* status_;

        QLabel* ctrl_;
        QLabel* alt_;
        QLabel* shift_;

        void clear();
        
        QString leftString();
        QString middleString();
        QString rightString();
};

#endif
