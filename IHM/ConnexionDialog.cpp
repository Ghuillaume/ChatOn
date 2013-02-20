#include "ConnexionDialog.hpp"

ConnexionDialog::ConnexionDialog(QWidget* parent):
    QDialog ( parent )
{

    this->setObjectName("Dialog");
    this->setWindowTitle(QString::fromUtf8("Connexion au serveur"));

    frame = new QWidget(this);
    frame->setObjectName("frame");

    formLayoutWidget = new QWidget(frame);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    formLayout = new QFormLayout(formLayoutWidget);
    formLayout->setObjectName(QString::fromUtf8("formLayout"));
    formLayout->setContentsMargins(10, 10, 10, 10);


    // Display labels
    pseudoLabel = new QLabel(formLayoutWidget);
    pseudoLabel->setObjectName("pseudoLabel");
    pseudoLabel->setText(QString::fromUtf8("Pseudo :"));
    formLayout->setWidget(0, QFormLayout::LabelRole, pseudoLabel);
    
    serverLabel = new QLabel(formLayoutWidget);
    serverLabel->setObjectName("serverLabel");
    serverLabel->setText(QString::fromUtf8("Serveur :"));
    formLayout->setWidget(1, QFormLayout::LabelRole, serverLabel);
    
    portLabel = new QLabel(formLayoutWidget);
    portLabel->setObjectName("portLabel");
    portLabel->setText(QString::fromUtf8("N° de port :"));
    formLayout->setWidget(2, QFormLayout::LabelRole, portLabel);


    // Display parameters edition

    pseudoEdit = new QLineEdit(formLayoutWidget);
    pseudoEdit->setObjectName(QString::fromUtf8("pseudoEdit"));
    formLayout->setWidget(0, QFormLayout::FieldRole, pseudoEdit);

    serverEdit = new QLineEdit(formLayoutWidget);
    serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
    formLayout->setWidget(1, QFormLayout::FieldRole, serverEdit);

    portEdit = new QLineEdit(formLayoutWidget);
    portEdit->setText(QString::fromUtf8("5555"));
    portEdit->setObjectName(QString::fromUtf8("portEdit"));
    formLayout->setWidget(2, QFormLayout::FieldRole, portEdit);


    pseudoEdit->setText("TestPseudo");
    serverEdit->setText("localhost");



    buttonBox = new QDialogButtonBox(formLayoutWidget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayout->setWidget(3, QFormLayout::FieldRole, buttonBox);

    QObject::connect(this->buttonBox, SIGNAL(accepted()), this, SLOT(checkFields()));
    QObject::connect(this, SIGNAL(acceptedAndOk()), this, SLOT(accept()));
    QObject::connect(this->buttonBox, SIGNAL(rejected()), this, SLOT(close()));

    formLayoutWidget->resize(formLayout->sizeHint());
    this->resize(formLayout->sizeHint());
}

ConnexionDialog::~ConnexionDialog()
{
    delete frame;
}

void ConnexionDialog::checkFields() {
    /*if(intituleEdit->text().isEmpty())
        QMessageBox::warning(this, "Erreur", "Prout");
    else*/
        emit acceptedAndOk();
}
