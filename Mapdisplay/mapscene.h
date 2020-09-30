#ifndef MAPSCENE_H
#define MAPSCENE_H

#include<QObject>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QMouseEvent>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsSceneWheelEvent>
#include<QDebug>
#include<QApplication>
#include<QDragMoveEvent>
#include<QRubberBand>
#include<QRect>
class MapScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MapScene(QObject*parent=nullptr);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
   void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;
    bool crop=false;



signals:

    void increaseZoom(QPointF clickPoint);
    void signalTargetCoordinate(QPointF point);
     void showPopUp(QPointF click);
     void mouse_Pressed();
     void mouse_Released();
private slots:


};

class MapView:public QGraphicsView
{

    Q_OBJECT
public:
    MapView(QWidget*parent=nullptr);
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) ;
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void mouseReleaseEvent(QMouseEvent *event);

     QPoint screenshotTopLeft,screenshotButtomRight;
     QRubberBand*rubberBand;
     bool rubber=false;

protected:
    void wheelEvent(QWheelEvent*event);
signals:
     void increaseZoom(QPointF clickPoint);
    void changeZoomWheel(bool zoom,QPointF wheelPoint);

};

#endif // MAPSCENE_H
