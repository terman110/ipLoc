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

protected:
    impData currentImg;
    FileDownloader* m_img;
    FileDownloader* m_source;
    QMenu* menu;
    QAction* a_close;
    QAction* a_restart;
    QAction* a_loc;
    QAction* a_ip;
    QAction* a_date;
    QUrl sourceUrl;
    int timerval;
    QString m_sSettingsFile;
    QTimer *timer;

    impData parseSource( QString src, QString needle);
    QString parseIp( QString src);

public:
    menuicon(QObject *parent = 0);

private slots:
    void loadImage();
    void loadSource();

    void onClose();
    void onRestart();
    void onDlg();
    void idle();
};

#endif // MENUICON_H
