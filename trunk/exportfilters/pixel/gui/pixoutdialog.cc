
#include "pixoutdialog.h"
#include "figure.h"

#include <QtGui>

PixoutDialog::PixoutDialog( PIXOutput* filter, QWidget* parent )
        : ExportDialog( parent ),
          filter_( filter ),
          keepAspectRatio_( true ),
          aspectRatio_( 1 )
{
        const Figure* figure = filter->figure_;
        QSizeF figSizeF = figure->boundingRect().size();
        figSize_ = figSizeF.toSize();
        
        aspectRatio_ = figSizeF.height() / figSizeF.width();

        QGridLayout* mainLayout = new QGridLayout( mainWidget_ );

        QGroupBox* sizeGroup = new QGroupBox();
        QGridLayout* sizeLayout = new QGridLayout( sizeGroup );

        xres = new QSpinBox( sizeGroup );
        xres->setValue( figSize_.width() );
        xres->setMinimum( 0 );
        xres->setSingleStep( 1 );
        QLabel* xresLabel = new QLabel( tr("&Width"), sizeGroup );
        xresLabel->setBuddy( xres );
        sizeLayout->addWidget( xresLabel, 0,0 );
        sizeLayout->addWidget( xres, 0, 1 );

        yres = new QSpinBox( sizeGroup );
        yres->setValue( figSize_.height() );
        yres->setMinimum( 0 );
        yres->setSingleStep( 1 );
        QLabel* yresLabel = new QLabel( tr("&Height"), sizeGroup );
        yresLabel->setBuddy( yres );
        sizeLayout->addWidget( yresLabel, 0,2 );
        sizeLayout->addWidget( yres, 0, 3 );

        scale = new QDoubleSpinBox( sizeGroup );
        scale->setValue( 100.0 );
        scale->setSuffix(" %");
        scale->setMinimum( 1 );
        scale->setSingleStep( 10 );
        QLabel* scaleLabel = new QLabel( tr("&Scale"), sizeGroup );
        scaleLabel->setBuddy( scale );
        sizeLayout->addWidget( scaleLabel, 1, 0 );
        sizeLayout->addWidget( scale, 1, 1 );

        keepAspect = new QCheckBox( tr("Keep &aspect ratio") );
        keepAspect->setCheckState( Qt::Checked );
        sizeLayout->addWidget( keepAspect, 1,2, 1,2 );

        mainLayout->addWidget( sizeGroup, 0,0 );

        connect( xres, SIGNAL( valueChanged( int ) ), this, SLOT( setXres( int ) ) );
        connect( yres, SIGNAL( valueChanged( int ) ), this, SLOT( setYres( int ) ) );
        connect( scale, SIGNAL( valueChanged( double ) ), this, SLOT( setScale( double ) ) );
        connect( keepAspect, SIGNAL( stateChanged( int ) ), 
                 this, SLOT( keepAspectRatio( int ) ) );
        
}

// ExportFilter* PNGFilterFactory::filter()
// {
//         return new PIXOutput("png");
// }

void PixoutDialog::setXres( int x )
{
        if ( keepAspect->isChecked() ) {
                int y = qRound( x * aspectRatio_ );
                yres->setValue( y );
                filter_->setYres( y );
                scale->setValue( 100 * x/figSize_.width() );
        }
        filter_->setXres( x );
}

void PixoutDialog::setYres( int y )
{
        if ( keepAspect->isChecked() ) {
                int x = qRound( y / aspectRatio_ );
                yres->setValue( x );
                filter_->setXres( x );
                scale->setValue( 100 * y/figSize_.height() );
        }
        filter_->setYres( y );
}


void PixoutDialog::setScale( double s )
{
        s /= 100;
        int x = qRound( s * figSize_.width() );
        int y = qRound( s * figSize_.height() );
        
        xres->setValue( x );
        yres->setValue( y );
        
        filter_->setXres( x );
        filter_->setYres( y );
}

void PixoutDialog::keepAspectRatio( int keep )
{
        if ( !keep )
                scale->setEnabled( false );
        keepAspectRatio_ = keep;
}
