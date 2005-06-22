
#include "pixoutdialog.h"
#include "figure.h"

#include <QtGui>

PixoutDialog::PixoutDialog( PIXOutput* filter, QWidget* parent )
        : ExportDialog( parent ),
          filter_( filter )
{
        const Figure* figure = filter->figure_;
        QSize figSize = figure->boundingRect().size().toSize();
        
        QGridLayout* mainLayout = new QGridLayout( mainWidget_ );

        QGroupBox* sizeGroup = new QGroupBox();
        QGridLayout* sizeLayout = new QGridLayout( sizeGroup );

        QSpinBox* xres = new QSpinBox( sizeGroup );
        xres->setValue( figSize.width() );
        xres->setMinimum( 0 );
        xres->setSingleStep( 1 );
        QLabel* xresLabel = new QLabel( tr("&Width"), sizeGroup );
        xresLabel->setBuddy( xres );
        sizeLayout->addWidget( xresLabel, 0,0 );
        sizeLayout->addWidget( xres, 0, 1 );

        QSpinBox* yres = new QSpinBox( sizeGroup );
        yres->setValue( figSize.height() );
        yres->setMinimum( 0 );
        yres->setSingleStep( 1 );
        QLabel* yresLabel = new QLabel( tr("&Height"), sizeGroup );
        yresLabel->setBuddy( yres );
        sizeLayout->addWidget( yresLabel, 0,2 );
        sizeLayout->addWidget( yres, 0, 3 );

        QDoubleSpinBox* scale = new QDoubleSpinBox( sizeGroup );
        scale->setValue( 100.0 );
        scale->setSuffix(" %");
        scale->setMinimum( 1 );
        scale->setSingleStep( 10 );
        QLabel* scaleLabel = new QLabel( tr("&Scale"), sizeGroup );
        scaleLabel->setBuddy( scale );
        sizeLayout->addWidget( scaleLabel, 1, 0 );
        sizeLayout->addWidget( scale, 1, 1 );

        QCheckBox* keepAspect = new QCheckBox( tr("Keep &aspect ratio") );
        keepAspect->setCheckState( Qt::Checked );
        sizeLayout->addWidget( keepAspect, 1,2, 1,2 );

        mainLayout->addWidget( sizeGroup, 0,0 );
}

// ExportFilter* PNGFilterFactory::filter()
// {
//         return new PIXOutput("png");
// }
