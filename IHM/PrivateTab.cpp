#include "PrivateTab.hpp"

PrivateTab::PrivateTab(QWidget *parent, Window *mainWindow, QString pseudo) :
    QWidget(parent)
{
    this->mainWindow = mainWindow;

    this->pseudo = pseudo;

    this->installEventFilter(this);
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

bool PrivateTab::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyRelease)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return) // Entrée du pavé numérique ou classique
        {
            textEntered();
            return true;
        }
    }
    return false;
}

void PrivateTab::textEntered() {
    QString texte = this->inputText->toPlainText();

    texte.remove('\n');

    if(!texte.isEmpty()) {
        this->inputText->clear();

        if(texte.startsWith('/')) {
            this->history->append(this->mainWindow->commandProcessing(texte));
        }
        else {
            this->history->append("Moi : " + texte);
            this->mainWindow->sendText(texte, this->mainWindow->tabWidget->tabText(this->mainWindow->tabWidget->indexOf(this)).toStdString());
        }
    }
}


void PrivateTab::addText(QString msg, QString pseudo) {

    this->history->append(pseudo + " : " + msg);
}
