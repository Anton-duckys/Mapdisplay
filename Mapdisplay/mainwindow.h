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
#include<QMessageBox>
#include<QtWebEngineWidgets>
#include<QtWebView>
#include<QtWebViewDepends>
#include<QWebEnginePage>
#include<QAbstractScrollArea>
#include<QScreen>
#include<mapscene.h>
#include"popup.h"
#include "maptile.h"
#include "filedownloader.h"
#include "scroller.h"
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
    QHBoxLayout*layout;
    double longitude (int x,int z, double picX);
    double latitude(int y,int z,double picY);

    double TwoGis_longitude (int x,int z, double picX);
    double TwoGis_latitude(int y,int z,double picY);

    std::pair<int,double> tileX(double lon,int z);
    std::pair<int,double> tileY(double lat,int z);

     std::pair<int,double> TwoGis_tileX(double lon,int z);
      std::pair<int,double> TwoGis_tileY(double lat,int z);

    bool intersected(QPointF firstTopLeft,QPointF firstBottomRight,QPointF secondTopLeft,QPointF secondBottomRight);
    void changeMapView(int zoom,QString zoomtype,QPointF viewTopCenter);
    void removeNonVisibleTiles();
    double remain(double number, double divider);
    QWebEngineView *webView;
    PopUp *popUp;


     bool eventFilter(QObject *watched, QEvent *event);
    std::pair<double,double> fromPointToCoordinate(QPointF targetPoint);
    void sceneItemPosChange(QString zoomtype,int zoom, QPointF targetPoint,QPointF prevCenter);

    QString google_type;



    scroller* m_scroller;

    void onClippedImage(const QPixmap& pixmap);
QRect selectionRect;
    QGraphicsPixmapItem* m_currentImageItem {nullptr};
    QPixmap originalPixmap;
    QLabel *screenshotLabel;




void shootScreen();
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
 void showPopUp(QPointF clickPoint);
 void startScroll();
 void endScroll();

        // Слот для обработки нажатия клавиши Enter
    void slotLinkClicked(QUrl url); // Слот для клика по ссылке на странице

    void on_actionRoads_Only_triggered();
    void on_actionStandard_triggered();
    void on_actionHybrid_triggered();
    void on_actionCrop_triggered();
};
#endif // MAINWINDOW_H
