#include "networkApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    networkApp w;
    w.show();
    return a.exec();
}
