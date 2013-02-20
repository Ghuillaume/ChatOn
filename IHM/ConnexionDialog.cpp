#include "ConnexionDialog.hpp"

ConnexionDialog::ConnexionDialog(QWidget* parent):
    QDialog ( parent )
{

	int width = 250;
	int height = 150;

    this->setObjectName("Dialog");
    this->resize(width, height);
    this->setWindowTitle(QString::fromUtf8("Connexion au serveur"));

    frame = new QWidget(this);
    frame->setObjectName("frame");
    //frame->setGeometry(QRect(10, 10, 500, 220));

    formLayoutWidget = new QWidget(frame);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
    //formLayoutWidget->setGeometry(QRect(10, 10, 450, 200));
    formLayoutWidget->setMinimumSize(width-30, height-30);
    formLayout = new QFormLayout(formLayoutWidget);
    //formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
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
    pseudoEdit->setText("TestLogin");
    formLayout->setWidget(0, QFormLayout::FieldRole, pseudoEdit);

    serverEdit = new QLineEdit(formLayoutWidget);
    serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
    formLayout->setWidget(1, QFormLayout::FieldRole, serverEdit);

    portEdit = new QLineEdit(formLayoutWidget);
    portEdit->setText(QString::fromUtf8("5555"));
    portEdit->setObjectName(QString::fromUtf8("portEdit"));
    formLayout->setWidget(2, QFormLayout::FieldRole, portEdit);



    buttonBox = new QDialogButtonBox(formLayoutWidget);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    formLayout->setWidget(3, QFormLayout::FieldRole, buttonBox);

    QObject::connect(this->buttonBox, SIGNAL(accepted()), this, SLOT(checkFields()));
    QObject::connect(this, SIGNAL(acceptedAndOk()), this, SLOT(accept()));
    QObject::connect(this->buttonBox, SIGNAL(rejected()), this, SLOT(close()));


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
