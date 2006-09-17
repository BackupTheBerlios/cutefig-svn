 
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

#ifndef fontwidget_h
#define fontwidget_h

#include <QWidget>
#include <QFont>
#include <QFontDatabase>
#include <QStringListModel>
#include <QItemSelectionModel>


class HeadListView;


class FontWidget : public QWidget
{
        Q_OBJECT
public:
        FontWidget( QWidget* parent = 0 );

        void setFont( QFont font );
	QFont font() const { return font_; }

signals:
        void fontChanged();

private slots:
	void changeFamily( const QString& fam );
	void changeSize( const QString& size );
	
private:
	void updateSizes();
	
private:
        QFont font_;
	QFontDatabase fontDatabase_;
	
	HeadListView* family_;
	HeadListView* size_;
};



class QLineEdit;
class QListView;

class HeadListView : public QWidget 
{
Q_OBJECT
public:
	HeadListView( const QString& title, QWidget* parent = 0);

	void setStringList( const QStringList& list );

	void select( const QString& itemstring );
	
signals:
	void highlightChanged( const QString& );

private slots:
	void updateHighLight( const QModelIndex& index, const QModelIndex& );
	
private:
	QLineEdit* head_;
	QListView* listView_;

	QStringListModel* model_;
	QItemSelectionModel* selectedOne_;
};

#endif
