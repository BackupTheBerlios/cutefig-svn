 
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
//        ~FontWidget();

        void setFont( QFont font );
	QFont font() const { return font_; }

signals:
        void fontChanged();

private slots:
	void changeFamily( const QString& fam );
	void changeSize( const QString& size );
	void changeStyle( const QString& style );
	
private:
	void updateSizes();
	void updateStyles();
	
private:
        QFont font_;
	QFontDatabase fontDatabase_;
	
	HeadListView* family_;
	HeadListView* size_;
	HeadListView* style_;
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


// class FontListView : public QListView 
// {
// public:
//         FontListView( QWidget* parent );
        
//         QStringListModel* model() const
//         {
//                 return static_cast<QStringListModel *>(QListView::model());
//         }
        
//         void setCurrentItem( int item )
//         {
//                 QListView::setCurrentIndex(static_cast<QAbstractListModel*>(model())->index(item));
//         }

//         int currentItem() const { return QListView::currentIndex().row(); }

//         int count() const { return model()->rowCount(); }

//         QString currentText() const
//         {
//                 int row = QListView::currentIndex().row();
//                 return row < 0 ? QString() : model()->stringList().at(row);
//         }

//         void currentChanged(const QModelIndex &current, const QModelIndex &previous)
//         {
//                 QListView::currentChanged(current, previous);
//                 if (current.isValid())
//                         emit highlighted(current.row());
//         }
        
//         QString text(int i) const { return model()->stringList().at(i); }
// signals:
//         void highlighted(int);
// };

#endif
