#ifndef MENUICON_H
#define MENUICON_H

#include "filedownload.h"

#include <QSystemTrayIcon>
#include <QMenu>
#include <QRegularExpression>

class menuicon : public QSystemTrayIcon
{
    Q_OBJECT

protected:
    QString m_lastIP;
    QMap<QString, QString> dictCountry;

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

    QRegularExpression* re;

    QList<QString> lstIcons;

    void fillCountryMap();
    void loadImage(QString countryCode);

public:
    menuicon(QObject *parent = 0);

private slots:
    void loadSource();

    void onClose();
    void onRestart();
    void onDlg();
    void idle();
};

#endif // MENUICON_H
