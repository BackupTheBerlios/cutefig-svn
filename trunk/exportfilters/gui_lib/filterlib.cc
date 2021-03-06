 
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

#include "filterlib.h"
#include "pixoutput.h"
#include "pixoutdialog.h"

#include <QApplication>

FilterLib::FilterLib()
        : filterHash_()
{
        filterHash_["png"] = new PNGFilterFactory;
        filterHash_["bmp"] = new BMPFilterFactory;
        FilterFactory* jpgff = new JPGFilterFactory;
        filterHash_["jpg"] = jpgff;
        filterHash_["jpeg"] = jpgff;
        filterHash_["xpm"] = new XPMFilterFactory;
        filterHash_["xbm"] = new XBMFilterFactory;
        filterHash_["ppm"] = new PPMFilterFactory;
}

QString FilterLib::filterString() const
{
        QString s;
        s+= qApp->translate("FilterLib", "Exportable formats:", "English Text") + " (";

        QHash<QByteArray,FilterFactory*>::const_iterator it = filterHash_.begin();
        for ( ; it != filterHash_.end(); ++it )
                s += "*." + it.key() + " ";
        s.remove( s.size()-1, 1 );
        s += ")";

        return s;
}
