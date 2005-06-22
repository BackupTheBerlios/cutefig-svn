/*****************************************************************************
**
**  @version $Id: pixoutput.h 8 2005-06-21 06:30:27Z joh $
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

#ifndef pixoutdialog_h
#define pixoutdialog_h

class PixoutDialog

class PNGFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("png"); }
};

class BMPFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("bmp"); }
};

class JPGFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("jpg"); }
};

class XPMFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("xpm"); }
};

class XBMFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("xbm"); }
};

class PPMFilterFactory : public FilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("ppm"); }
};


#endif
