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

#include <iostream>

#include "PrivateTab.hpp"


#define LABEL_HEIGHT 20
#define PEOPLE_LIST_WIDTH 200

#define HISTORY_WIDTH (APP_WIDTH - PEOPLE_LIST_WIDTH - 3 * MARGIN)


using namespace std;

class Window : public QMainWindow
{
    Q_OBJECT
    
	public:
		explicit Window(QWidget *parent = 0);
		~Window();

        void addConnected(string name);
        void removeConnected(string name);

	private:
		QWidget *centralWidget;
		
		QMenuBar* menubar;
		QMenu* fileMenu;
		QAction* quitItem;
		
		QTabWidget* tabWidget;
		QWidget* mainTab;

        QTextEdit* history;
        QTextEdit* inputText;
        QListWidget* connectedPeople;
        QLabel* nbPeople;
        QPushButton* sendButton;

    public slots:
        void newTab(QListWidgetItem *itemClicked);
        void sendText();
};

#endif // WINDOW_HPP
