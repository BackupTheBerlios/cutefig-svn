 
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

#ifndef CUTEFIG_H
#define CUTEFIG_H

#include <QMainWindow>
#include <QSize>

class Figure;
class CanvasView;
class Controler;

/** \class CuteFig
 *  \brief The \e actual application. It inherits from QMainWindow.
 */

class CuteFig: public QMainWindow
{
        Q_OBJECT

public:
        CuteFig(); //!< Called from within main()
        ~CuteFig() {}
        
        Controler* controler() const { return controler_; }
        CanvasView* canvasView() const { return cview_; }

        QSize viewportSize() const { return viewport_->size(); }

protected:
        void closeEvent( QCloseEvent* );


private slots:
        void init(); //! performs the initialisation that cannot be done in the constructor
        void newDoc(); //!< a new document has been opened 
        void choose(); //!< lets the user choose a document to open
        void load( const QString& fileName ); //!< opens a chosen document
        void save();   
        void saveAs();
        void exportFigure();
        void print();

        void about();  //!< Displays the about box.

private:
        CanvasView *cview_;        //!< The editing view of the figure
        Controler *controler_;     //!< The controler editing the figure
        Figure *figure_;           //!< The figure acts as the model

        QString filename_;         //!< Name of the file being edited

        QWidget* viewport_;

//        void setupObjectMapper();  //!< sets up the ObjectMapper
        void setupActions(); //!< sets up the standard actions

        void readSettings();
        void writeSettings();
        
public:
        static int rulerWidth;
        static double unit;
};


#endif
