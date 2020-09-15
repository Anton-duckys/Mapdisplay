#include "myqgraphicsview.h"

myQGraphicsView::myQGraphicsView(QWidget * parent)
: QGraphicsView(parent) {}

myQGraphicsView::myQGraphicsView(QGraphicsScene * scene, QWidget * parent)
: QGraphicsView(scene, parent) {}

void myQGraphicsView::wheelEvent(QWheelEvent * event)
{

}
