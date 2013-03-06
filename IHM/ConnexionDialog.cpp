#include "ConnexionDialog.hpp"

ConnexionDialog::ConnexionDialog(QWidget* parent, char *ip):
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

    ipLabel = new QLabel(formLayoutWidget);
    ipLabel->setObjectName("ipLabel");
    ipLabel->setText(QString::fromUtf8("IP locale :"));
    formLayout->setWidget(1, QFormLayout::LabelRole, ipLabel);
    
    serverLabel = new QLabel(formLayoutWidget);
    serverLabel->setObjectName("serverLabel");
    serverLabel->setText(QString::fromUtf8("Serveur :"));
    formLayout->setWidget(2, QFormLayout::LabelRole, serverLabel);
    
    portLabel = new QLabel(formLayoutWidget);
    portLabel->setObjectName("portLabel");
    portLabel->setText(QString::fromUtf8("N° de port :"));
    formLayout->setWidget(3, QFormLayout::LabelRole, portLabel);


    // Display parameters edition

    pseudoEdit = new QLineEdit(formLayoutWidget);
    pseudoEdit->setObjectName(QString::fromUtf8("pseudoEdit"));
    formLayout->setWidget(0, QFormLayout::FieldRole, pseudoEdit);

    ipEdit = new QLineEdit(formLayoutWidget);
    ipEdit->setObjectName(QString::fromUtf8("ipEdit"));
    formLayout->setWidget(1, QFormLayout::FieldRole, ipEdit);

    serverEdit = new QLineEdit(formLayoutWidget);
    serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
    formLayout->setWidget(2, QFormLayout::FieldRole, serverEdit);

    portEdit = new QLineEdit(formLayoutWidget);
    portEdit->setText(QString::fromUtf8("5555"));
    portEdit->setObjectName(QString::fromUtf8("portEdit"));
    formLayout->setWidget(3, QFormLayout::FieldRole, portEdit);

    serverEdit->setText("localhost");
    ipEdit->setText(QString::fromAscii(ip));


    buttonBox = new QDialogButtonBox(formLayoutWidget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayout->setWidget(4, QFormLayout::FieldRole, buttonBox);

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

    if(pseudoEdit->text().length() < 2) {
        QMessageBox::warning(this, "Erreur", QString::fromUtf8("Votre pseudo doit contenir au moins 2 caractères"), QMessageBox::Ok);
    }
    else if(ipEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", QString::fromUtf8("Veuillez spécifier votre adresse IP"), QMessageBox::Ok);
    }
    else if(portEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", QString::fromUtf8("Veuillez spécifier le port distant"), QMessageBox::Ok);
    }
    else if(serverEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", QString::fromUtf8("Veuillez spécifier l'adresse du serveur distant"), QMessageBox::Ok);
    }
    else {
        emit acceptedAndOk();
    }
}
