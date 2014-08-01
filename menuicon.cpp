#include "menuicon.h"

#include <QMessageBox>
#include <QTimer>

menuicon::menuicon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    this->setIcon(QIcon());

    menu = new QMenu("ipLoc");
    a_close = menu->addAction("Quit");
    connect(a_close, SIGNAL(triggered()), this, SLOT(onClose()));
    this->setContextMenu( menu);

    idle();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(idle()));
    timer->start(10000);
}

menuicon::~menuicon()
{

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

    src = src.remove(0,src.indexOf("</head>"));
    impData loc = parseSource(src, "class=\"location\"");
    QString ip = parseIp(src);
    menu = new QMenu("ipLoc");
    QAction* a_loc = menu->addAction( loc.txt);
    a_loc->setEnabled(false);
    QAction* a_ip = menu->addAction( ip);
    a_ip->setEnabled(false);
    QAction* a_space = menu->addAction("");
    a_space->setEnabled(false);
    QAction* a_copy = menu->addAction( "Copyright by Jan Beneke");
    a_copy->setEnabled(false);
    QAction* a_url = menu->addAction( "http://www.lightgraffiti.de");
    a_url->setEnabled(false);
    QAction* a_use = menu->addAction( "Using http://www.wieistmeineip.de");
    a_use->setEnabled(false);
    a_close = menu->addAction("Quit");
    connect(a_close, SIGNAL(triggered()), this, SLOT(onClose()));
    this->setContextMenu( menu);

    m_img = new FileDownloader(loc.url, this);
    connect(m_img, SIGNAL(downloaded()), SLOT(loadImage()));
}

void menuicon::idle()
{
    QUrl sourceUrl("http://www.wieistmeineip.de/");
    m_source = new FileDownloader(sourceUrl, this);
    connect(m_source, SIGNAL(downloaded()), SLOT(loadSource()));
}

void menuicon::onClose()
{
    exit(0);
}

impData menuicon::parseSource( QString src, QString needle)
{
    impData data;

    src = src.remove(0,src.indexOf(needle) + needle.length() + 1);
    src.truncate(src.indexOf("</div>"));
    qDebug() << src;

    needle = "src=\"";
    QString img = src.remove(0,src.indexOf(needle) + needle.length());
    img.truncate(img.indexOf("\""));
    qDebug() << img;
    data.url = img;

    needle = "/";
    QString sho = img.remove(0,img.lastIndexOf(needle) + needle.length());
    sho.truncate(sho.indexOf("."));
    sho = sho.toUpper();
    qDebug() << sho;
    data.sho = sho;

    needle = "alt=\"";
    QString txt = src.remove(0,src.indexOf(needle) + needle.length());
    txt.truncate(txt.indexOf("\""));
    qDebug() << txt;
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
    qDebug() << src;

    return src;
}
