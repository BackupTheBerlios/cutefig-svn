
#include "gradientdialog.h"
#include "gradientwidget.h"
#include "flagbuttongroup.h"
#include "strokeiconengines.h"

#include "reslib.h"
#include "stroke.h"

#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QListWidget>
#include <QIcon>

#include <QDebug>

GradientDialog::GradientDialog( const Gradient* gradient, QWidget* parent )
        : EditDialog( parent ),
          gradient_( ( gradient ) ? gradient->copy() : 0 ),
          oldGradient_( gradient_ ),
          radius_( 0.2 )
{        
        QHBoxLayout* mainLayout = new QHBoxLayout();
        
        gradWidget_ = new GradientWidget( gradient_, this );
        gradWidget_->setMinimumSize( QSize( 300,200 ) );
        mainLayout->addWidget( gradWidget_ );

        QGroupBox* typeGroupBox = new QGroupBox( tr("Gradient &type"), this );
        mainLayout->addWidget( typeGroupBox );

        QVBoxLayout* typeBoxLayout = new QVBoxLayout( typeGroupBox );

        FlagButtonGroup* gradType = new FlagButtonGroup( typeGroupBox );
        
        QRadioButton* linear = new QRadioButton( tr("&Linear"), typeGroupBox );
        QRadioButton* radial = new QRadioButton( tr("Ra&dial"), typeGroupBox );

        gradType->addButton( linear, int( Gradient::Linear ) );
        gradType->addButton( radial, int( Gradient::Radial ) );
        
        typeBoxLayout->addWidget( linear );
        typeBoxLayout->addWidget( radial );

        if ( gradient_ ) 
                if ( gradient_->type() == Gradient::Linear )
                        linear->setChecked( true );
                else
                        radial->setChecked( true );
        
        connect( gradType, SIGNAL( stateChanged(int) ), this, SLOT( typeChanged(int) ) );

        gradListWgt_ = new QListWidget( this );
        fillGradList();
        connect( gradListWgt_, SIGNAL( itemActivated(QListWidgetItem*) ),
                 this, SLOT( changeGradientFromList(QListWidgetItem*) ) );
        mainLayout->addWidget( gradListWgt_ );
        
        dialogLayout_->insertLayout( 0, mainLayout );
}

void GradientDialog::reset()
{
        delete gradient_;
        gradient_ = oldGradient_->copy();
        gradWidget_->setGradient( gradient_ );
}

void GradientDialog::typeChanged( int type )
{
        Gradient* ng;

        if ( Gradient::Type( type ) == Gradient::Linear ) {
                ng = new LinearGradient( gradient_->firstPoint(), gradient_->secondPoint() );
                RadialGradient* rg = static_cast<RadialGradient*>( gradient_ );
                radius_ = rg->radius();
        }
        else
                ng = new RadialGradient(gradient_->firstPoint(),gradient_->secondPoint(),radius_);

        ng->setColorStops( gradient_->colorStops() );
        gradWidget_->setGradient( ng );
        delete gradient_;
        gradient_ = ng;
}

void GradientDialog::fillGradList()
{
        StrokeLib& sl = StrokeLib::instance();

        foreach( ResourceKey key, sl.keys() ) {
                const Stroke& s = sl[key];
                qDebug() << key.keyString();
                if ( s.type() == Stroke::sGradient ) {
                        QListWidgetItem* wi = new QListWidgetItem( key.keyString(), gradListWgt_ );
                        wi->setIcon( QIcon( new GradientIconEngine( s.gradient() ) ) );
                        gradHash_[wi] = key;
                }
        }
}

void GradientDialog::changeGradientFromList( QListWidgetItem* wi )
{
        Gradient* gr = StrokeLib::instance()[gradHash_[wi]].gradient();
        gradient_ = gr;
        
//         if ( gradient_->type() == Gradient::Linear )
//                 linear->setChecked( true );
//         else
//                 radial->setChecked( true );

        gradWidget_->setGradient( gradient_ );
}
