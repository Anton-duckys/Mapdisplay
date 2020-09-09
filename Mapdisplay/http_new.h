#ifndef HTTP_NEW_H
#define HTTP_NEW_H
#include <QtGui/QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QBuffer>
#include <QHttp>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ui_http_new.h"

class http_new : public QMainWindow
{
    Q_OBJECT

public:
    http_new(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~http_new();

public slots:
    void finishedSlot(QNetworkReply* reply);

private:
    Ui::http_newClass ui;
    QImage * image;
        QPixmap screenImage;
        QPainter * p;
    QBuffer *buffer;
    QByteArray bytes;
    QHttp *http;
    int Request;
};
#endif // HTTP_NEW_H
