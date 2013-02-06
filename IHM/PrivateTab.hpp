#ifndef PRIVATETAB_HPP
#define PRIVATETAB_HPP

#include <QWidget>
#include <QApplication>
#include <QMainWindow>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>

#include <string>
#include <iostream>

using namespace std;


#define APP_WIDTH 900
#define APP_HEIGHT 600
#define TAB_HEIGHT (APP_HEIGHT - 55)
#define MARGIN 10
#define SENDING_HEIGHT 70
#define BUTTON_WIDTH 100

#define PRIVATE_HISTORY_WIDTH (APP_WIDTH - 2 * MARGIN)
#define HISTORY_HEIGHT (TAB_HEIGHT - SENDING_HEIGHT - 3 * MARGIN)
#define SENDING_WIDTH (APP_WIDTH - BUTTON_WIDTH - 3 * MARGIN)

class PrivateTab : public QWidget
{
    Q_OBJECT
    public:
        explicit PrivateTab(QWidget *parent = 0, QTabWidget* tabWidget = NULL);

        void setObjects();

    private:
        QTabWidget* tabWidget;

        QTextEdit* history;
        QTextEdit* inputText;
        QPushButton* sendButton;


    signals:

    public slots:
        void sendText();
    
};

#endif // PRIVATETAB_HPP
