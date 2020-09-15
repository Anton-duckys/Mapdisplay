#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
#include<QLabel>
#include<QPixmap>
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QBuffer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "filedownloader.h"
#include<QGraphicsScene>
#include<QGraphicsView>
#include "maptile.h"
#include<QVector>
#include<QPushButton>
#include<QTimer>
#include<QtMath>
#include<QTransform>
#include<QSslSocket>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QLabel *label;
    QGraphicsScene *scene;
    QBuffer *buffer;
    MapTile *tile;
    QNetworkAccessManager* nam;
    int Request;
    QImage*image;
    FileDownloader*m_pImgCtrl;
    QList<MapTile*>tiles;
    int tilesamount;
    int size;
    int zoom;
    double SceneX,SceneY;
    QPushButton *decreaseZoom;
    QPushButton *increaseZoom;
    void changeMapView(int zoom);
    QTimer* centerChanged;
    QTimer* updateTimer;
    int count;
    QVector<QVector<bool>>bool_tiles;
    bool intersected(QPointF firstTopLeft,QPointF firstBottomRight,QPointF secondTopLeft,QPointF secondBottomRight);
    bool issetted=true;
    int sceneSize;
    QString api ="https://tile.openstreetmap.org/";
    QUrl urlpai;
    double remain(double number, double divider);


    QPointF sceneTopLeft;
    QPointF sceneBottomRight;
    QVector<QVector<bool>> check_tiles;
    qreal longitude (int x,int z);
    qreal latitude(int y,int z);
    int tileX(qreal lon,int z);
    int tileY(qreal lat,int z);



    void removeNonVisibleTiles();




signals:
    void zoomChanged(int value);

private slots:
 void loadImage(int number,int x,int y);
 void showCenter();
 void on_pushButton_clicked();
 void on_pushButton_2_clicked();
 void on_actionOpen_Streen_Map_triggered();
 void on_actionYandex_Map_triggered();

 void on_pushButton_3_clicked();
};
#endif // MAINWINDOW_H
