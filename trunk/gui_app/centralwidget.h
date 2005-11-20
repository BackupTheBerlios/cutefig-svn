 
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


#ifndef centralwidget_h
#define centralwidget_h

#include <QWidget>

class QMainWindow;

class Ruler;
class CanvasView;

/** \class CentralWidget
 *  \brief The central widget of the CuteFig.  
 */

class CentralWidget : public QWidget
{
        Q_OBJECT
public:
        CentralWidget( CanvasView* cv, QMainWindow * parent =0 );
        ~CentralWidget() {}

        QWidget* viewport() const { return viewport_; }

private slots:
        void resizeRulers(); //!< resizes the Rulers

private:
        void resizeEvent( QResizeEvent* );

        Ruler *hRuler_, *vRuler_;  //!< Two rulers to indicate the position
        QWidget* viewport_;        //!< The scroll views's viewport
        
};

inline void CentralWidget::resizeEvent( QResizeEvent* )
{
        resizeRulers();
}


#endif
