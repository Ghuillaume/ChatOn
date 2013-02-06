#include "Window.hpp"

Window::Window(QWidget *parent, int serverSocket, string pseudo) : QMainWindow(parent)
{
    this->serverSocket = serverSocket;
    this->pseudo = pseudo;

    //this->resize(APP_WIDTH, APP_HEIGHT);
    this->setFixedSize(APP_WIDTH, APP_HEIGHT);
    QString title = "ChatOn - ";
    title += pseudo.c_str();
    this->setWindowTitle(title);


    /* Menubar */
    menubar = new QMenuBar(this);
    this->setMenuBar(menubar);
    menubar->setObjectName("menubar");

    fileMenu = new QMenu(menubar);
    fileMenu->setObjectName("fileMenu");
    menubar->addAction(fileMenu->menuAction());
    fileMenu->setTitle(QString::fromUtf8("Fichier"));

        quitItem = new QAction(this);
        quitItem->setShortcut(QKeySequence::Quit);
        quitItem->setObjectName("quitItem");
        fileMenu->addAction(quitItem);
        quitItem->setText("Quitter");
        QObject::connect(quitItem, SIGNAL(triggered()), this, SLOT(close()));
        QObject::connect(this, SIGNAL(destroyed()), this, SLOT(close()));
        

    // Central widget
    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralwidget"));
    centralWidget->setMinimumSize(APP_WIDTH, APP_HEIGHT);
    this->setCentralWidget(centralWidget);

	// Tab Widget
 	tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    tabWidget->setGeometry(0, 0, APP_WIDTH, APP_HEIGHT);
    mainTab = new QWidget(tabWidget);
    mainTab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(mainTab, QString());
    tabWidget->setTabText(tabWidget->indexOf(mainTab), QApplication::translate("MainWindow", "Salon principal", 0, QApplication::UnicodeUTF8));
    tabWidget->setTabsClosable(true);

    // Onglet principal : salon général
    history = new QTextEdit(mainTab);
    history->setObjectName(QString::fromUtf8("history"));
    history->setGeometry(QRect(MARGIN, MARGIN, HISTORY_WIDTH, HISTORY_HEIGHT));
    history->setReadOnly(true);
    connectedPeople = new QListWidget(mainTab);
    connectedPeople->setObjectName(QString::fromUtf8("connectedPeople"));
    connectedPeople->setGeometry(QRect(HISTORY_WIDTH + 2*MARGIN, 2*MARGIN+LABEL_HEIGHT, PEOPLE_LIST_WIDTH, HISTORY_HEIGHT-30));
    nbPeople = new QLabel(mainTab);
    nbPeople->setObjectName(QString::fromUtf8("nbPeople"));
    nbPeople->setText(QString::fromUtf8("0 personnes connectées"));
    nbPeople->setGeometry(QRect(HISTORY_WIDTH + 2*MARGIN, MARGIN, PEOPLE_LIST_WIDTH, LABEL_HEIGHT));
    inputText = new QTextEdit(mainTab);
    inputText->setObjectName(QString::fromUtf8("inputText"));
    inputText->setGeometry(QRect(MARGIN, HISTORY_HEIGHT + 2*MARGIN, SENDING_WIDTH, SENDING_HEIGHT));
    sendButton = new QPushButton(mainTab);
    sendButton->setObjectName(QString::fromUtf8("sendButton"));
    sendButton->setGeometry(QRect(SENDING_WIDTH + 2*MARGIN, HISTORY_HEIGHT + 2*MARGIN, BUTTON_WIDTH, SENDING_HEIGHT));
    sendButton->setText("Envoyer");

    QObject::connect(connectedPeople, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(newTab(QListWidgetItem*)));
    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(textEntered()));

}

Window::~Window()
{

}

void Window::addConnected(string name) {
    this->connectedPeople->insertItem(this->connectedPeople->count(), QString::fromUtf8(name.c_str()));

    QString nb = QString::number(this->connectedPeople->count());
    nb += QString::fromUtf8(" personnes connectées");
    this->nbPeople->setText(nb);
}

void Window::removeConnected(string name) {
    for(int i = 0 ; i < this->connectedPeople->count() ; i++) {
        if(this->connectedPeople->item(i)->text().toStdString() == name)
            this->connectedPeople->takeItem(i);
    }

    QString nb = QString::number(this->connectedPeople->count());
    nb += QString::fromUtf8(" personnes connectées");
    this->nbPeople->setText(nb);
}

void Window::newTab(QListWidgetItem* itemClicked) {

    string title = itemClicked->text().toStdString();

    // Création d'un nouvel onglet
    PrivateTab* tab = new PrivateTab(tabWidget, this);
    tab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(tab, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", title.c_str(), 0, QApplication::UnicodeUTF8));
    tab->setObjects();

    this->tabWidget->setCurrentWidget(tab);

}

void Window::textEntered() {
    QString texte = this->inputText->toPlainText();

    if(!texte.isEmpty()) {
        this->inputText->clear();

        if(texte.startsWith('/')) {
            this->history->append(this->commandProcessing(texte));
        }
        else {
            this->history->append("Moi : " + texte);
            sendText(texte, "all");
        }
    }
}

void Window::sendText(QString text, string dest) {

    QString toSend = "";
    if(dest == "all") {
        toSend += "/all ";
    }
    else {
        toSend += "/msg ";
        toSend += dest.c_str();
        toSend += " ";
    }

    toSend += text;

    std::cout << "Sending : " << toSend.toStdString().c_str() << std::endl;
    write(this->serverSocket,toSend.toStdString().c_str(),strlen(toSend.toStdString().c_str()));
}


QString Window::commandProcessing(QString text) {

    QString result = "";

    if(text == "/help") {
        result += QString::fromUtf8("/help : afficher l'aide ; /quit : se déconnecter");
    }
    else if(text == "/quit") {
        this->close();
    }
    else {
        result += QString::fromUtf8("Commande inconnue");
    }

    return result;
}

void Window::close() {
    std::cout << "Closing..." << endl;

    write(this->serverSocket, "/quit", 6);
    // TODO close socket
    exit(0);
}
