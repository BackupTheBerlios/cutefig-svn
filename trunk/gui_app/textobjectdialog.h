 
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

#ifndef textobjectdialog_h
#define textobjectdialog_h

#include "objectdialog.h"

class TextObject;
class FontWidget;
class StrokeWidget;

class TextObjectDialog : public ObjectDialog
{
        Q_OBJECT
public:
        TextObjectDialog( DrawObject* o, EditdialogAction* a, QWidget* parent );
        ~TextObjectDialog() {}

private:
        virtual void commitChangesPrivate();
        virtual void setUpConnectionsPrivate();
        virtual void setDefaultValuesPrivate();
        virtual void castDrawObject();

        TextObject* textObject_;

        FontWidget* fontWidget_;
};

#endif
