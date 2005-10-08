#ifndef actionpushbutton_h
#define actionpushbutton_h

#include <QPushButton>
#include <QAction>

class ActionPushButton : public QPushButton
{
        Q_OBJECT
public:
        ActionPushButton( QAction* action, QWidget* parent = 0 );
        ~ActionPushButton() {}

private:
        QAction* action_;

private slots:
        void actionChanged();
};


#endif
