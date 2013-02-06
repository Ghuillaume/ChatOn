#include "Window.hpp"

Window::Window(QWidget *parent) : QMainWindow(parent)
{
    //this->resize(APP_WIDTH, APP_HEIGHT);
    this->setFixedSize(APP_WIDTH, APP_HEIGHT);

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
    QObject::connect(sendButton, SIGNAL(clicked()), this, SLOT(sendText()));

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
    PrivateTab* tab = new PrivateTab(tabWidget, tabWidget);
    tab->setObjectName(QString::fromUtf8("tab"));
    tabWidget->addTab(tab, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", title.c_str(), 0, QApplication::UnicodeUTF8));
    tab->setObjects();

    this->tabWidget->setCurrentWidget(tab);

}

void Window::sendText() {
    QString texte = this->inputText->toPlainText();

    if(!texte.isEmpty()) {
        std::cout << "Send to all : " << texte.toStdString().c_str() << std::endl;
        this->history->append("Moi : " + this->inputText->toPlainText());
        this->inputText->clear();
    }
}
