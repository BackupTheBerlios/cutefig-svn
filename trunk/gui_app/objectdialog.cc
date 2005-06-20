 
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

#include <QtGui>

#include "cutefig.h"

#include "drawobject.h"
#include "pen.h"
#include "reslib.h"
#include "objectdialog.h"
#include "progutils.h"
#include "colorbutton.h"
#include "stylecombobox.h"
#include "lineshowwidget.h"
#include "flagbuttongroup.h"

#include "editdialogaction.h"


ObjectDialog::ObjectDialog( DrawObject* o, EditdialogAction* a,
                            QWidget* parent )
        : QDialog( parent ),
          action_ ( a )
{ 
        qDebug("ObjectDialog::ObjectDialog");
        drawObject_ = o;

        QVBoxLayout* dialogLayout = new QVBoxLayout( this );
        QHBoxLayout* bottomLayout = new QHBoxLayout();

        tabWidget = new QTabWidget( this );

        QPushButton* ok = new QPushButton( tr("&Ok"), this );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        ok->setAutoDefault( true );

        QPushButton* cancel = new QPushButton( tr("Cancel"), this );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

        QPushButton* reset = new QPushButton( tr("&Reset"), this );
        connect( reset, SIGNAL( clicked() ), this, SLOT( resetObject() ) );
        
        bottomLayout->addWidget( reset );
        bottomLayout->addStretch();
        bottomLayout->addWidget( ok );
        bottomLayout->addWidget( cancel );

        dialogLayout->addWidget( tabWidget );
        dialogLayout->addLayout( bottomLayout );
} 

void ObjectDialog::setUpGeneral()
{
        qDebug("ObjectDialog::setUpGeneral");

        QFrame* page = new QFrame();
        tabWidget->addTab( page, "&General" );

        QVBoxLayout* topLayout = new QVBoxLayout( page );
//        ProgUtils::prepareLayout( topLayout, false );
    
        // final setup

        topLayout->addWidget( setUpLineGroup( page ) );

        topLayout->addWidget( setUpFillGroup( page ) );

        QHBoxLayout* dlt = new QHBoxLayout( topLayout->widget() );
        topLayout->addItem( dlt );
        depth = new QSpinBox( page );
        depth->setMinimum(0);
        depth->setMaximum(999);
        depth->setSingleStep(1);
        QLabel* dlb = new QLabel( tr("&Depth"), page );
        dlb->setBuddy( depth );
        dlt->addWidget( dlb );
        dlt->addWidget( depth );

        // Comment

        QVBoxLayout* clt = new QVBoxLayout( topLayout->widget() );
        topLayout->addItem( clt );
        comment = new QTextEdit( page );
//        comment->setTextFormat( Qt::PlainText );
        comment->setTabChangesFocus( true );
        QLabel* clb = new QLabel( tr("&Comment"), page );
        clb->setBuddy( comment );
        clt->addWidget( clb );
        clt->addWidget( comment );
}

QGroupBox* ObjectDialog::setUpLineGroup( QWidget* page )
{
        qDebug("ObjectDialog::setUpLineGroup");
        QGroupBox* lineGroup = new QGroupBox( tr("&Pen"), page );
        QGridLayout* lineLayout = new QGridLayout( lineGroup );

        lineWidth = new QDoubleSpinBox( lineGroup );
        lineWidth->setRange( 1.0, 10.0 );
        lineWidth->setSingleStep( 0.5 );

        QLabel* lwl = new QLabel( tr("&Width"),lineGroup );
        lwl->setBuddy( lineWidth );
        lineLayout->addWidget( lwl, 0,0 );
        lineLayout->addWidget( lineWidth, 0,1 );
       
        lineColor = new ColorButton( Qt::black, lineGroup ); 
        QLabel* lcl = new QLabel( tr("&Color"),lineGroup );
        lcl->setBuddy( lineColor );
        lineLayout->addWidget( lcl, 1,0 );
        lineLayout->addWidget( lineColor, 1,1 );
             
   
        lineStyle = new StyleComboBox( lineGroup );
        QLabel* lsl = new QLabel( tr("&Style"),lineGroup );
        lsl->setBuddy( lineStyle );
        lineLayout->addWidget( lsl, 2,0 );
        lineLayout->addWidget( lineStyle, 2,1 );

        lineLayout->addItem( new QSpacerItem( 10, 0 ), 0,2 );


        QGroupBox* capGroup = new QGroupBox( tr("Ca&p style"), lineLayout->widget() );
        QHBoxLayout* capLayout = new QHBoxLayout( capGroup );

        capStyle = new FlagButtonGroup( capGroup );

        QRadioButton* capFlat = new QRadioButton( capGroup );
        QRadioButton* capRound = new QRadioButton( capGroup );
        QRadioButton* capSquare = new QRadioButton( capGroup );

        capFlat->setIcon( QIcon(":images/cap_flat") );
        capRound->setIcon( QIcon(":images/cap_round") );
        capSquare->setIcon( QIcon(":images/cap_square") );

        capStyle->addButton( capFlat, int( Qt::FlatCap ) );
        capStyle->addButton( capRound, int( Qt::RoundCap ) );
        capStyle->addButton( capSquare, int( Qt::SquareCap ) );

        capLayout->addWidget( capFlat );
        capLayout->addWidget( capRound );
        capLayout->addWidget( capSquare );

        lineLayout->addWidget( capGroup, 0,3 );


        QGroupBox* joinGroup = new QGroupBox( tr("&Join style") );
        QHBoxLayout* joinLayout = new QHBoxLayout( joinGroup );

        joinStyle = new FlagButtonGroup( joinGroup );

        QRadioButton* joinMiter = new QRadioButton( joinGroup );
        QRadioButton* joinBevel = new QRadioButton( joinGroup );
        QRadioButton* joinRound = new QRadioButton( joinGroup );

        joinMiter->setIcon( QIcon(":images/join_miter") );
        joinBevel->setIcon( QIcon(":images/join_bevel") );
        joinRound->setIcon( QIcon(":images/join_round") );

        joinStyle->addButton( joinMiter, int( Qt::MiterJoin ) );
        joinStyle->addButton( joinBevel, int( Qt::BevelJoin ) );
        joinStyle->addButton( joinRound, int( Qt::RoundJoin ) );

        joinLayout->addWidget( joinMiter );
        joinLayout->addWidget( joinBevel );
        joinLayout->addWidget( joinRound );

        lineLayout->addWidget( joinGroup, 1,3 );


        lineShow = new LineShowWidget( drawObject_->pen(), 40, lineGroup );
        lineLayout->addWidget( lineShow, 2,3 );

        return lineGroup;
}

QGroupBox* ObjectDialog::setUpFillGroup( QWidget* page )
{
        qDebug("ObjectDialog::setUpFillGroup");
        QGroupBox* fillGroup = new QGroupBox( tr("&Fill"), page );
        QGridLayout* fillLayout = new QGridLayout( fillGroup );
        ProgUtils::prepareLayout( fillLayout );

        fillColor = new ColorButton( Qt::black, fillGroup );
        QLabel* fgLabel = new QLabel( tr("C&olor"),fillGroup );
        fgLabel->setBuddy( fillColor );
        fillLayout->addWidget( fgLabel, 0,0 );
        fillLayout->addWidget( fillColor, 0,1 );

//        fillPattern = new BrushButton( fillGroup );
//        fillPattern->setFixedHeight( 40 );
//        fillLayout->addWidget( new QLabel( fillPattern, tr("Pa&ttern"),
//                                           fillGroup ), 1,0 );
//        fillLayout->addWidget( fillPattern, 1,1 );

        return fillGroup;
    
}

void ObjectDialog::accept()
{
        if ( comment->document()->isModified() ) {
                drawObject_->setComment( comment->toPlainText() );
                action_->wObjectHasChanged();
        }

        QDialog::accept();
}

void ObjectDialog::resetObject()
{
        DrawObject* o = action_->restoreWObject();
        if ( !o ) {
                qDebug("WARNING !!! restoring wObject failed...");
                return;
        }

        drawObject_ = o;

        lineShow->disconnect();
//        fillPattern->disconnect();
        depth->disconnect();

        setDefaultValues();
        setUpConnections();
}


void ObjectDialog::setDefaultValues()
{ 
        qDebug("ObjectDialog::setDefaultValues");

        const Pen& pen = drawObject_->pen();

        lineWidth->setValue( pen.width() );
        lineColor->setColor( pen.color() );
        qDebug() << pen.dashesKey();

        lineStyle->setCurrentIndex( pen.dashesKey() );

        capStyle->setState( int( pen.capStyle() ) );
        joinStyle->setState( int( pen.joinStyle() ) );

        lineShow->setColor( pen.color() );
        lineShow->setWidth( pen.width() );

        fillColor->setColor( drawObject_->fillColor() );

        depth->setValue( drawObject_->depth() );
        comment->setPlainText( drawObject_->comment() );
}



void ObjectDialog::setUpConnections()
{
        qDebug("ObjectDialog::setUpConnections");

        connect( lineShow, SIGNAL( changed( const Pen& ) ),
                 drawObject_, SLOT( setPen( const Pen& ) ) );
    
        connect( lineWidth, SIGNAL( valueChanged(double) ), lineShow, SLOT( setWidth(double) ) );
        connect( lineColor, SIGNAL( colorChanged(QColor) ), lineShow, SLOT( setColor(QColor) ) );
        connect( lineStyle, SIGNAL( highlightedDash(int) ), lineShow, SLOT( setDashes(int) ) );
        connect( capStyle,  SIGNAL( stateChanged(int) ),    lineShow, SLOT( setCapStyle(int) ) );
        connect( joinStyle, SIGNAL( stateChanged(int) ),    lineShow, SLOT( setJoinStyle(int ) ) );

//        connect( fillColor, SIGNAL( colorChanged( QColor ) ),
//                 fillPattern, SLOT( changeFillColor( QColor ) ) );
//        connect( lineColor, SIGNAL( colorChanged( QColor ) ),
//                 fillPattern, SLOT( changePenColor( QColor ) ) );
    
//        connect( fillColor, SIGNAL(colorChanged(QColor)), drawObject_, SLOT(setFillColor(QColor)) );

//        connect( fillPattern, SIGNAL( brushChanged(int) ),
//                 drawObject_, SLOT( setPattern(int) ) );

//        connect( fillPattern, SIGNAL( brushIsNoBrush( bool ) ),
//                 fillColor, SLOT( setDisabled( bool ) ) );

        connect( depth, SIGNAL( valueChanged(int) ), drawObject_, SLOT( setDepth(int) ) );

        connect( depth, SIGNAL( valueChanged( int ) ), action_, SLOT( wObjectHasChanged() ) );
        connect( lineShow, SIGNAL( changed( const Pen& ) ), action_, SLOT( wObjectHasChanged() ) );
//        connect( fillPattern, SIGNAL( changed() ),
//                 controler_, SLOT( wObjectHasChanged() ) );
}

void ObjectDialog::setUpAll()
{
        setUpGeneral();
        setUpPrivate();
        setDefaultValues();
        setUpConnections();
}
