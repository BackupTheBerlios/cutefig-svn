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

#ifndef pixoutdialog_h
#define pixoutdialog_h

#include "editdialog.h"
#include "filterfactory.h"
#include "pixoutput.h"

#include <QSize>

class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class ChainButton;
class QSlider;


template<typename Resource> class ResourceButton;
typedef ResourceButton<QColor> ColorButton;

class PixoutDialog : public EditDialog
{
        Q_OBJECT
public:
        PixoutDialog( PIXOutput* filter, QWidget* parent=0 );
        ~PixoutDialog() {}

private:
        PIXOutput* filter_;
        bool keepAspectRatio_;
        double aspectRatio_;
        QSizeF figSize_;

        QSpinBox *xres_, *yres_;
        QDoubleSpinBox *scale_;
        ChainButton *keepAspect_;
        ColorButton* bgColor_;
	QDoubleSpinBox* gamma_;
	QSlider* quality_;
	QCheckBox* withPaper_;

	void setupInitialValues();
	void commitChanges();
	void doReset() {}

	void updateWidgets();
	void applyChanges();

	void calcFigSize( bool withPaper );
};

class PixFilterFactory : public FilterFactory
{
public:
        PixoutDialog* dialog( ExportFilter* filter, QWidget* parent=0 ) {
                return new PixoutDialog( (PIXOutput*) filter, parent );
        }
};

class PNGFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("png"); }
};

class BMPFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("bmp"); }
};

class JPGFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("jpg"); }
};

class XPMFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("xpm"); }
};

class XBMFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("xbm"); }
};

class PPMFilterFactory : public PixFilterFactory
{
public:
        ExportFilter* filter() { return new PIXOutput("ppm"); }
};


#endif
