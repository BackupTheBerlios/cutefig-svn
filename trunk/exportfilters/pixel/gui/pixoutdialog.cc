
#include "pixoutdialog.h"
#include "figure.h"
#include "colorbutton.h"

#include <QtGui>

#include <limits>

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
        
        setWindowTitle( tr("CuteFig: export to a bitmap graphics:") );
        
        QGridLayout* mainLayout = new QGridLayout( topLayout_->widget() );

        QGroupBox* sizeGroup = new QGroupBox( this );
        QGridLayout* sizeLayout = new QGridLayout( sizeGroup );

        xres = new QSpinBox( sizeGroup );
        xres->setValue( figSize_.width() );
        xres->setRange( 0, std::numeric_limits<int>::max() );
        xres->setSingleStep( 1 );
        QLabel* xresLabel = new QLabel( tr("&Width"), sizeGroup );
        xresLabel->setBuddy( xres );
        sizeLayout->addWidget( xresLabel, 0,0 );
        sizeLayout->addWidget( xres, 0, 1 );

        sizeLayout->addItem( new QSpacerItem( 20, 0 ), 0, 2 );
        
        yres = new QSpinBox( sizeGroup );
        yres->setValue( figSize_.height() );
        yres->setRange( 0, std::numeric_limits<int>::max()  );
        yres->setSingleStep( 1 );
        QLabel* yresLabel = new QLabel( tr("&Height"), sizeGroup );
        yresLabel->setBuddy( yres );
        sizeLayout->addWidget( yresLabel, 0,3 );
        sizeLayout->addWidget( yres, 0, 4 );

        scale = new QDoubleSpinBox( sizeGroup );
        scale->setSuffix(" %");
        scale->setRange( std::numeric_limits<double>::min(), std::numeric_limits<double>::max() );
        scale->setValue( 100.0 );
        scale->setSingleStep( 10 );
        QLabel* scaleLabel = new QLabel( tr("&Scale"), sizeGroup );
        scaleLabel->setBuddy( scale );
        sizeLayout->addWidget( scaleLabel, 1, 0 );
        sizeLayout->addWidget( scale, 1, 1 );

        keepAspect = new QCheckBox( tr("Keep &aspect ratio") );
        keepAspect->setCheckState( Qt::Checked );
        sizeLayout->addWidget( keepAspect, 1,3, 1,2 );

        mainLayout->addWidget( sizeGroup, 0,0 );

        QHBoxLayout* bgLayout = new QHBoxLayout( mainLayout->widget() );
        QColor bg( Qt::white );
        bg.setAlpha( 0 );
        ColorButton* bgColor = new ColorButton( bg, this );
        QLabel* bgLabel = new QLabel( tr("&Background color"), this );
        bgLabel->setBuddy( bgColor );
        bgLayout->addWidget( bgLabel );
        bgLayout->addWidget( bgColor );
        
        

        mainLayout->addItem( bgLayout, 1,0 );
        
        connect( xres, SIGNAL( valueChanged( int ) ), this, SLOT( setXres( int ) ) );
        connect( yres, SIGNAL( valueChanged( int ) ), this, SLOT( setYres( int ) ) );
        connect( scale, SIGNAL( valueChanged( double ) ), this, SLOT( setScale( double ) ) );
        connect( keepAspect, SIGNAL( stateChanged( int ) ), 
                 this, SLOT( keepAspectRatio( int ) ) );
        connect( bgColor, SIGNAL( colorChanged(QColor) ), this, SLOT( setBackground(QColor) ) );

        topLayout_->addItem( mainLayout );
        setupStandardButtons();
}

void setValue( QSpinBox* sb, int value )
{
        sb->blockSignals( true );
        sb->setValue( value );
        sb->blockSignals( false );
}

void setValue( QDoubleSpinBox* sb, double value )
{
        sb->blockSignals( true );
        sb->setValue( value );
        sb->blockSignals( false );
}


void PixoutDialog::setXres( int x )
{
        if ( keepAspect->isChecked() ) {
                int y = qRound( x * aspectRatio_ );
                if ( y <= 0 ) {
                        setValue( xres, qRound(yres->value() / aspectRatio_) );
                        return;
                }
                setValue( yres, y );
                filter_->setYres( y );
                setValue( scale, 100 * x/figSize_.width() );
        }
        filter_->setXres( x );
}

void PixoutDialog::setYres( int y )
{
        if ( keepAspect->isChecked() ) {
                int x = qRound( y / aspectRatio_ );
                if ( x <= 0 ) {
                        setValue( yres, qRound(xres->value() * aspectRatio_) );
                        return;
                }
                xres->setValue( x );
                filter_->setXres( x );
                setValue( scale, 100 * y/figSize_.height() );
        }
        filter_->setYres( y );
}


void PixoutDialog::setScale( double s )
{
        s /= 100;
        int x = qRound( s * figSize_.width() );
        int y = qRound( s * figSize_.height() );

        if ( x < 0 && y < 0 ) {
                setValue( scale, xres->value() / figSize_.width() * 100 );
                return;
        }
        
        setValue( xres, x );
        setValue( yres, y );
        
        filter_->setXres( x );
        filter_->setYres( y );
}

void PixoutDialog::keepAspectRatio( int keep )
{
        scale->setEnabled( keep );
        keepAspectRatio_ = keep;
}

void PixoutDialog::setBackground( QColor color )
{
        filter_->setBackground( color );
}
