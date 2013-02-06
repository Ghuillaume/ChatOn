#include "PrivateTab.hpp"

PrivateTab::PrivateTab(QWidget *parent, QTabWidget* tabWidget) :
    QWidget(parent)
{
    this->tabWidget = tabWidget;


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

    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(sendText()));
}

void PrivateTab::sendText() {
    QString texte = this->inputText->toPlainText();

    if(!texte.isEmpty()) {
        std::cout << "Send to " << this->tabWidget->tabText(tabWidget->indexOf(this)).toStdString().c_str() << " : " << this->inputText->toPlainText().toStdString().c_str() << std::endl;
        this->history->append("Moi : " + this->inputText->toPlainText());
        this->inputText->clear();
    }
}
