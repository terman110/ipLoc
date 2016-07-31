#include "menuicon.h"
#include "filedownload.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    a.setApplicationDisplayName("ipLoc");
    a.setApplicationName("ipLoc");
    a.setApplicationVersion("1.2");
    a.setObjectName("lg.ipLoc");
    a.setOrganizationDomain("Lightgraffiti");
    a.setOrganizationName("Jan Beneke");
    a.setWindowIcon(QIcon(":/ipLoc_icon.png"));

    menuicon* tray = new menuicon(&a);
    tray->show();

    return a.exec();
}
