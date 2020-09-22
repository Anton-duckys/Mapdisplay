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
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QVector>
#include<QPushButton>
#include<QTimer>
#include<QtMath>
#include<QTransform>
#include<QSslSocket>
#include<QScrollBar>
#include<QLayout>
#include<mapscene.h>

#include "maptile.h"
#include "filedownloader.h"
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
    MapScene *scene;
    MapView*graphicsView;
    MapTile *tile;
    QNetworkAccessManager* nam;
    QImage*image;
    QList< MapTile*>tiles;
    QList<MapTile*>temp_tiles;
    int tilesamount;
    int size;
    int zoom;
    bool load_tem;
    int tilesInView;
    double SceneX,SceneY;
    QTimer* centerChanged; 
    bool issetted=true;
    int sceneSize;
    QString api ="https://tile2.maps.2gis.com/";
    QUrl urlpai; 
    QVector<QVector<bool>> check_tiles;

    double longitude (int x,int z, double picX);
    double latitude(int y,int z,double picY);

    double TwoGis_longitude (int x,int z, double picX);
    double TwoGis_latitude(int y,int z,double picY);

    int tileX(double lon,int z);
    int tileY(double lat,int z);

    bool intersected(QPointF firstTopLeft,QPointF firstBottomRight,QPointF secondTopLeft,QPointF secondBottomRight);
    void changeMapView(int zoom,QString zoomtype,QPointF viewTopCenter);
    void removeNonVisibleTiles();
    double remain(double number, double divider);




signals:
    void zoomChanged(int value);

private slots:
 void loadImage(int x,int y);
 void showCenter();
 void on_pushButton_clicked();
 void on_pushButton_2_clicked();
 void on_actionOpen_Streen_Map_triggered();
 void on_actionYandex_Map_triggered();
 void increaseZoomByDoubleClick(QPointF clickPoint);
 void changeZoomByWheel(bool zoom, QPointF wheelPoint);
 void slotTargetCoordinate(QPointF target);
 void on_action2Gis_triggered();
};
#endif // MAINWINDOW_H
