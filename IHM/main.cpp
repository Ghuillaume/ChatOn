#include <QtGui/QApplication>
#include "Window.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Window w;
    w.show();

    w.addConnected("machin");
    w.addConnected("truc");
    w.addConnected("bidule");
    w.addConnected("chouette");
    w.removeConnected("truc");

    return a.exec();
}
