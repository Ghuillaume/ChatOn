#ifndef _CONNEXIONDIALOG_HPP_
#define _CONNEXIONDIALOG_HPP_

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QMessageBox>

using namespace std;

class ConnexionDialog: public QDialog
{
    Q_OBJECT

    public:
        ConnexionDialog(QWidget *parent, char* ip);
       ~ConnexionDialog();

        QWidget *formLayoutWidget;
        QFormLayout *formLayout;
        
        QDialogButtonBox *buttonBox;

        QLineEdit* pseudoEdit;
        QLineEdit* ipEdit;
        QLineEdit* serverEdit;
        QLineEdit* portEdit;

    private:

        QLabel* pseudoLabel;
        QLabel* ipLabel;
		QLabel* serverLabel;
		QLabel* portLabel;
		
        QWidget *frame;

    public slots:
        void checkFields();

    signals:
        void acceptedAndOk();
};

#endif

