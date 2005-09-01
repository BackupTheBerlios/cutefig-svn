#ifndef editdialog_h
#define editdialog_h

#include <QDialog>

class QVBoxLayout;

class EditDialog : public QDialog
{
        Q_OBJECT
public:
        EditDialog( QWidget* parent = 0 );
        ~EditDialog() {}

protected slots:
        virtual void reset() = 0;

protected:
        QVBoxLayout* dialogLayout_;
};

#endif
