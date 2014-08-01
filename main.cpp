#include "menuicon.h"
#include "filedownload.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    menuicon* tray = new menuicon(&a);
    tray->show();

    return a.exec();
}
