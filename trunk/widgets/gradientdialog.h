#ifndef gradientdialog_h
#define gradientdialog_h


#include "editdialog.h"
#include "resourcekey.h"

#include <QHash>

class Gradient;
class GradientWidget;
class QListWidget;
class QListWidgetItem;

class GradientDialog : public EditDialog
{
        Q_OBJECT
public:
        GradientDialog( const Gradient* gradient, QWidget* parent = 0 );
        ~GradientDialog() {}

        Gradient* gradient() const { return gradient_; }
        
private slots:
        void typeChanged( int type );
        void changeGradientFromList( QListWidgetItem* );
        virtual void reset();
        
private:
        Gradient *gradient_, *oldGradient_;
        GradientWidget* gradWidget_;

        QListWidget* gradListWgt_;
        
        double radius_;

        QHash<QListWidgetItem*,ResourceKey> gradHash_;

        void fillGradList();
};

#endif
