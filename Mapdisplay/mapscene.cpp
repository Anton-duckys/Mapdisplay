#include "mapscene.h"



MapScene::MapScene(QObject *parent):QGraphicsScene()
{

}

void MapScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit increaseZoom(event->scenePos());
}

void MapScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}





MapView::MapView(QWidget *parent):QGraphicsView()
{
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MapView::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit increaseZoom(event->scenePos());
    qDebug()<<"Mapview double click"<<endl;
}

void MapView::wheelEvent(QWheelEvent *event)
{

    if(event->delta()>=0){
        qDebug()<<"My view"<<event->delta()<<endl;
        //emit changeZoomByWheel(true,event->scenePos());
         emit changeZoomWheel(true,mapToScene( QPoint(event->pos().x(),event->pos().y())));


    }
    else
         //emit changeZoomByWheel(false,event->scenePos());
    emit changeZoomWheel(false,mapToScene( QPoint(event->pos().x(),event->pos().y())));


}
