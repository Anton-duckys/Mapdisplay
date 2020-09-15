#include "maptile.h"

MapTile::MapTile(QPixmap pixmap, QGraphicsItem *parent)
{
    setPixmap(pixmap);
}

MapTile::~MapTile()
{
   // qDebug()<<"Tile is deleted"<<endl;
}
