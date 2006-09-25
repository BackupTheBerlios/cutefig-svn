 
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

#ifndef layouter_h
#define layouter_h

class QString;
class QBoxLayout;
class QGridLayout;
class QWidget;

//! A Conveniance class to layout labeled widgets
/*! The Layouter can be used to layout labeled widgets side by side in
 *  very little code lines.  The usual way of using it is the
 *  following using only one statement.
 */
class Layouter
{
public:
        explicit Layouter( QBoxLayout* layout );

        Layouter& labeledWidget( const QString& text, QWidget* widget, int stretch = 0 );
        Layouter& widget( QWidget* w, int stretch = 0 );
        Layouter& stretch( int strech = 0 );
        void finishTo( QBoxLayout* target );
        void finishTo( QBoxLayout* target, int index );
        void finishTo( QGridLayout* target, int row, int col );
        
private:
        QBoxLayout* layout_;
};      

#endif
