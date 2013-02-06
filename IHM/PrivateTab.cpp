#include "PrivateTab.hpp"

PrivateTab::PrivateTab(QWidget *parent, Window *mainWindow) :
    QWidget(parent)
{
    this->mainWindow = mainWindow;


}

void PrivateTab::setObjects() {
    history = new QTextEdit(this);
    history->setObjectName(QString::fromUtf8("history"));
    history->setGeometry(QRect(MARGIN, MARGIN, PRIVATE_HISTORY_WIDTH, HISTORY_HEIGHT));
    history->setReadOnly(true);
    inputText = new QTextEdit(this);
    inputText->setObjectName(QString::fromUtf8("inputText"));
    inputText->setGeometry(QRect(MARGIN, HISTORY_HEIGHT + 2*MARGIN, SENDING_WIDTH, SENDING_HEIGHT));
    sendButton = new QPushButton(this);
    sendButton->setObjectName(QString::fromUtf8("sendButton"));
    sendButton->setGeometry(QRect(SENDING_WIDTH + 2*MARGIN, HISTORY_HEIGHT + 2*MARGIN, BUTTON_WIDTH, SENDING_HEIGHT));
    sendButton->setText("Envoyer");

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(textEntered()));
}

void PrivateTab::textEntered() {
    QString texte = this->inputText->toPlainText();

    if(!texte.isEmpty()) {
        this->inputText->clear();

        if(texte.startsWith('/')) {
            this->history->append(this->mainWindow->commandProcessing(texte));
        }
        else {
            this->history->append("Moi : " + this->inputText->toPlainText());
            this->mainWindow->sendText(texte, this->mainWindow->tabWidget->tabText(this->mainWindow->tabWidget->indexOf(this)).toStdString());
        }
    }
}
