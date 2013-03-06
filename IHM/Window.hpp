#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <map>

#include <iostream>
#include <unistd.h>

#include "PrivateTab.hpp"


#define LABEL_HEIGHT 20
#define PEOPLE_LIST_WIDTH 200

#define HISTORY_WIDTH (APP_WIDTH - PEOPLE_LIST_WIDTH - 3 * MARGIN)


using namespace std;

class PrivateTab;
typedef map<string, PrivateTab*> ListOfTab;
typedef pair<string, PrivateTab*> Tab;

class Window : public QMainWindow
{
    Q_OBJECT
    
	public:
        explicit Window(QWidget *parent = 0, int serverSocket = 0, string pseudo = "Guest");
		~Window();

        void addConnected(string name);
        void removeConnected(string name);

        void sendText(QString text, string dest = "");
        void receiveText(const char* msg, const char* pseudo, bool isPrivate);
        QString commandProcessing(QString text);


        QTabWidget* tabWidget;
        ListOfTab listOfTabs;

	private:
        int serverSocket;
        string pseudo;

		QWidget *centralWidget;
		
		QMenuBar* menubar;
		QMenu* fileMenu;
		QAction* quitItem;

		QWidget* mainTab;

        QTextEdit* history;
        QTextEdit* inputText;
        QListWidget* connectedPeople;
        QLabel* nbPeople;
        QPushButton* sendButton;

    public slots:
        void newTab(QListWidgetItem *itemClicked);
        void textEntered();
        void close();
        void closeEvent(QCloseEvent *event);
        bool eventFilter(QObject *obj, QEvent *event);
};

#endif // WINDOW_HPP
