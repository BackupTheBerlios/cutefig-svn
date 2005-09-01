
#include "editdialog.h"

#include <QLayout>
#include <QPushButton>

EditDialog::EditDialog( QWidget* parent )
        : QDialog( parent ),
          dialogLayout_( new QVBoxLayout( this ) )
{
        QHBoxLayout* bottomLayout = new QHBoxLayout();

        QPushButton* ok = new QPushButton( tr("&Ok"), this );
        connect( ok, SIGNAL( clicked() ), this, SLOT( accept() ) );
        ok->setAutoDefault( true );

        QPushButton* cancel = new QPushButton( tr("Cancel"), this );
        connect( cancel, SIGNAL( clicked() ), this, SLOT( reject() ) );

        QPushButton* reset = new QPushButton( tr("&Reset"), this );
        connect( reset, SIGNAL( clicked() ), this, SLOT( reset() ) );
        
        bottomLayout->addWidget( reset );
        bottomLayout->addStretch();
        bottomLayout->addWidget( ok );
        bottomLayout->addWidget( cancel );

        dialogLayout_->insertLayout( 1, bottomLayout );
}
