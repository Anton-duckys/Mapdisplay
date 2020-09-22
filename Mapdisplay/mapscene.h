#ifndef MAPSCENE_H
#define MAPSCENE_H

#include<QObject>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QMouseEvent>
#include<QGraphicsSceneMouseEvent>
#include<QGraphicsSceneWheelEvent>
#include<QDebug>
class MapScene:public QGraphicsScene
{
    Q_OBJECT
public:
    MapScene(QObject*parent=nullptr);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);



signals:

    void increaseZoom(QPointF clickPoint);
    void signalTargetCoordinate(QPointF point);


};

class MapView:public QGraphicsView
{

    Q_OBJECT
public:
    MapView(QWidget*parent=nullptr);
     void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

protected:
    void wheelEvent(QWheelEvent*event);
signals:
     void increaseZoom(QPointF clickPoint);
    void changeZoomWheel(bool zoom,QPointF wheelPoint);
};

#endif // MAPSCENE_H
