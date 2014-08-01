#ifndef MENUICON_H
#define MENUICON_H

#include "filedownload.h"

#include <QSystemTrayIcon>
#include <QMenu>

struct impData{
    QUrl url;
    QString txt;
    QString sho;
};

class menuicon : public QSystemTrayIcon
{
    Q_OBJECT

    QMenu* menu;

    FileDownloader* m_img;
    FileDownloader* m_source;

    QAction* a_close;

    impData parseSource( QString src, QString needle);
    QString parseIp( QString src);

public:
    menuicon(QObject *parent = 0);
    ~menuicon();

private slots:
    void loadImage();
    void loadSource();

    void onClose();
    void idle();
};

#endif // MENUICON_H
