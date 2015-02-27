#include "menuicon.h"
#include "dlg.h"

#include <QMessageBox>
#include <QTimer>
#include <QDateTime>
#include <QSettings>
#include <QApplication>

menuicon::menuicon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    timerval = QSettings().value("timer value", 10000).toInt();

    this->setIcon(QIcon());

    menu = new QMenu("ipLoc");
    a_loc = menu->addAction( "--");
    a_loc->setEnabled(false);
    a_ip = menu->addAction( "xx.xx.xx.xx");
    a_ip->setEnabled(false);
    a_date = menu->addAction( "Last updated: never");
    a_date->setEnabled(false);
    QAction* a_space = menu->addAction("");
    a_space->setEnabled(false);
    QAction* a_dlg = menu->addAction( "Settings");
    connect(a_dlg, SIGNAL(triggered()), this, SLOT(onDlg()));
    a_restart = menu->addAction("Restart");
    connect(a_restart, SIGNAL(triggered()), this, SLOT(onRestart()));
    a_close = menu->addAction("Quit");
    connect(a_close, SIGNAL(triggered()), this, SLOT(onClose()));
    this->setContextMenu( menu);

    timer = new QTimer(this);
    timer->setInterval(timerval);
    timer->start(timerval);
    //timer->setTimerType(Qt::PreciseTimer);
    connect(timer, SIGNAL(timeout()), this, SLOT(idle()));

    sourceUrl = "http://www.wieistmeineip.de/";
    m_source = new FileDownloader(sourceUrl, this);
    connect(m_source, SIGNAL(downloaded()), SLOT(loadSource()));

    QUrl imgUrl("http://static.wie-ist-meine-ip.de/img/country/cn.png");
    m_img = new FileDownloader(imgUrl, this);
    connect(m_img, SIGNAL(downloaded()), SLOT(loadImage()));

    idle();
}

void menuicon::loadImage()
{
    QPixmap buttonImage;
    buttonImage.loadFromData(m_img->downloadedData());
    this->setIcon(QIcon(buttonImage));
    this->show();
}

void menuicon::loadSource()
{
    QString src = m_source->downloadedString();
    //m_source->setUrl(sourceUrl);

    src = src.remove(0,src.indexOf("</head>"));
    impData loc = parseSource(src, "class=\"location\"");
    QString ip = parseIp(src);

    QTime time = QTime::currentTime();

    a_loc->setText(loc.txt);
    a_ip->setText(ip);
    a_date->setText("Last updated: " + time.toString());

    if( currentImg.url.fileName().compare( loc.url.fileName()) != 0)
        m_img->setUrl( loc.url);
    currentImg = loc;
}

void menuicon::idle()
{
    m_source->setUrl(sourceUrl);
    //loadSource();
    //loadImage();
}

void menuicon::onDlg()
{
    timer->stop();
    dlg* m_dlg = new dlg( &timerval);
    m_dlg->setModal( true);
    m_dlg->show();
    m_dlg->exec();

    QSettings().setValue("timer value", timerval);

    onRestart();
}

void menuicon::onClose()
{
    QSettings().setValue("timer value", timerval);
    timer->stop();
    QApplication::quit();
}

void menuicon::onRestart()
{
    timer->stop();
    timer->setInterval(timerval);
    timer->start(timerval);
}

impData menuicon::parseSource( QString src, QString needle)
{
    impData data;

    src = src.remove(0,src.indexOf(needle) + needle.length() + 1);
    src.truncate(src.indexOf("</div>"));
    //qDebug() << src;

    needle = "src=\"";
    QString img = src.remove(0,src.indexOf(needle) + needle.length());
    img.truncate(img.indexOf("\""));
    //qDebug() << img;
    data.url = img;

    needle = "/";
    QString sho = img.remove(0,img.lastIndexOf(needle) + needle.length());
    sho.truncate(sho.indexOf("."));
    sho = sho.toUpper();
    //qDebug() << sho;
    data.sho = sho;

    needle = "alt=\"";
    QString txt = src.remove(0,src.indexOf(needle) + needle.length());
    txt.truncate(txt.indexOf("\""));
    //qDebug() << txt;
    data.txt = txt;

    return data;
}

QString menuicon::parseIp( QString src)
{
    QString needle = "id=\"ipv4\" class=\"ip\"";
    src = src.remove(0,src.indexOf(needle) + needle.length() + 1);
    src.truncate(src.indexOf("</strong>"));
    needle = "<strong";
    src = src.remove(0,src.indexOf(needle) + needle.length() + 1);
    //qDebug() << src;

    return src;
}
