 
/*****************************************************************************
**
**  $Id$
**
**  This file is part of CuteFig
**
**  Copyright (C) 2004 Johannes Mueller, johmue@users.sourceforge.net
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

#ifndef gradientdialog_h
#define gradientdialog_h


#include "editdialog.h"
#include "resourcekey.h"
#include "gradient.h"

#include <QHash>

class GradientWidget;
class FlagButtonGroup;
// class QListWidget;
// class QListWidgetItem;

class GradientDialog : public EditDialog
{
        Q_OBJECT
public:
        GradientDialog( const Gradient& gradient, QWidget* parent = 0 );
        ~GradientDialog() {}

        Gradient gradient() const { return gradient_; }

        static Gradient getGradient( const Gradient& initial, bool *ok, QWidget* parent = 0 );
        
private slots:
        void typeChanged( int type );
//        void changeGradientFromList( QListWidgetItem* );
        virtual void reset();
        
private:
        Gradient gradient_, oldGradient_;
        GradientWidget* gradWidget_;
        FlagButtonGroup* gradType_;
//        QListWidget* gradListWgt_;
        
//        double radius_;

//        QHash<QListWidgetItem*,ResourceKey> gradHash_;

//        void fillGradList();
};

#endif
