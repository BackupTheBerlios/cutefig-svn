 
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

#include "fontwidget.h"

#include <QtGui>


HeadListView::HeadListView( const QString& title, QWidget* parent ) 
	: QWidget( parent )
{
	head_ = new QLineEdit;
	listView_ = new QListView;

	QLabel* tLabel = new QLabel( title );
	tLabel->setBuddy( listView_ );

	model_ = new QStringListModel( this );
	selectedOne_ = new QItemSelectionModel( model_ );
	
	listView_->setModel( model_ );
	listView_->setEditTriggers( QAbstractItemView::NoEditTriggers );
	listView_->setSelectionModel( selectedOne_ );
	
	QVBoxLayout* l = new QVBoxLayout;

	l->addWidget( tLabel );
	l->addWidget( head_ );
	l->addWidget( listView_ );

	setLayout( l );

	connect( selectedOne_, SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ),
		 this, SLOT( updateHighLight( const QModelIndex&, const QModelIndex& ) ) );
}

void HeadListView::setStringList( const QStringList& list )
{
	model_->setStringList( list );
}

void HeadListView::updateHighLight( const QModelIndex& index, const QModelIndex& ) 
{
	QString s = model_->data( index, Qt::DisplayRole ).toString();
	
	head_->setText( s );
	emit highlightChanged( s );
}


void HeadListView::select( const QString& itemString )
{
	int ind = model_->stringList().indexOf( itemString );
	const QModelIndex mi = model_->index( ind );

	if ( !mi.isValid() )
		return;

	selectedOne_->select( mi, QItemSelectionModel::ClearAndSelect );
	updateHighLight( mi, QModelIndex() );
	listView_->scrollTo( mi );
}

	




FontWidget::FontWidget( QWidget* parent )
	: QWidget( parent ),
	  font_(),
	  fontDatabase_( QFontDatabase() )
{
	family_ = new HeadListView( tr("Font &family") );
	size_ = new HeadListView( tr("Font &size") );

	QHBoxLayout* l = new QHBoxLayout;
	l->addWidget( family_, 38 );
	l->addWidget( size_, 10 );
	setLayout( l );

	family_->setStringList( fontDatabase_.families() );

	connect( family_, SIGNAL( highlightChanged( const QString& ) ),
		 this, SLOT( changeFamily( const QString& ) ) );
	connect( size_, SIGNAL( highlightChanged( const QString& ) ),
		 this, SLOT( changeSize( const QString& ) ) );
}


void FontWidget::changeFamily( const QString& fam )
{
	font_.setFamily( fam );
	updateSizes();
	
	emit fontChanged();
}

void FontWidget::changeSize( const QString& size )
{
	font_.setPointSize( size.toInt() );

	emit fontChanged();
}


void FontWidget::updateSizes()
{
	QStringList sizes;
	foreach ( int size, fontDatabase_.pointSizes( font_.family() ) )
		sizes << QString::number( size );

	size_->setStringList( sizes );    
}

void FontWidget::setFont( QFont font )
{
	font_ = font;
	updateSizes();
	family_->select( font_.family() );
	size_->select( QString::number( font_.pointSize() ) );
}

