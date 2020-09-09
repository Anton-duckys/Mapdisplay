#ifndef MAPTILE_H
#define MAPTILE_H

#include<QGraphicsRectItem>
#include<QPixmap>
#include<QObject>
#include<QGraphicsPixmapItem>
#include<QDebug>
class MapTile:public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MapTile(QPixmap pixmap, QGraphicsItem*parent=0);
    ~MapTile();
};

#endif // MAPTILE_H
