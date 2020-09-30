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

void MapScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons()==Qt::RightButton)
       {emit showPopUp(event->scenePos());
    }
//if(event->buttons()==Qt::LeftButton)
   // emit mouse_Pressed();


}

void MapScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   // emit mouse_Released();

}





MapView::MapView(QWidget *parent):QGraphicsView()
{
    //this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    viewport()->setCursor(Qt::CrossCursor);


}

void MapView::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<"xdfgdffgdgf"<<endl;
    rubber=true;
    screenshotTopLeft = event->pos();
        if (!rubberBand)
            rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
        rubberBand->setGeometry(QRect(screenshotTopLeft, QSize()));
        rubberBand->show();
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{if(rubber)
rubberBand->setGeometry(QRect( screenshotTopLeft, event->pos()).normalized());
}

void MapView::mouseReleaseEvent(QMouseEvent *event)
{
    rubber=false;
rubberBand->hide();
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
