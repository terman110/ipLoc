#include "filedownload.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent) :
    QObject(parent)
{
    this->setUrl(imageUrl);
}

FileDownloader::~FileDownloader()
{

}

void FileDownloader::setUrl( QUrl imageUrl)
{
    this->disconnect(&m_WebCtrl);
    this->connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)),
                SLOT(fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}

QString FileDownloader::downloadedString() const
{
    return QString( this->downloadedData());
}
