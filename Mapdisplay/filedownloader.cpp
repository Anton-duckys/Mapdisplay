#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl,int x,int y, QObject *parent) :
 QObject(parent)
{
    this->number=number;
    this->x=x;
    this->y=y;
    //qDebug()<<"FileDownloader constructed"<<endl;
 connect(
  &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
  this, SLOT (fileDownloaded(QNetworkReply*))
  );

 QNetworkRequest request(imageUrl);
 m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() {
    //qDebug()<<this->x<<" and "<<this->y<<" tile is deleted"<<endl;
}

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {

  //  qDebug()<<"Data is reading"<<endl;
 m_DownloadedData = pReply->readAll();
 //emit a signal
 pReply->deleteLater();
 emit downloaded(this->x,this->y);
}

QByteArray FileDownloader::downloadedData() const {
 return m_DownloadedData;
}
