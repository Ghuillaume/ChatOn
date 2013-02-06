#include <QtGui/QApplication>
#include "Window.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;
    w.show();
    
    return a.exec();
}
