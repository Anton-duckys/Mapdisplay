#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include<QGraphicsView>

class myQGraphicsView: public QGraphicsView
{

public:
    myQGraphicsView(QWidget * parent = nullptr);
    myQGraphicsView(QGraphicsScene * scene, QWidget * parent = nullptr);

protected:
   // virtual void wheelEvent(QWheelEvent * event);
    virtual void wheelEvent(QWheelEvent *event) override;
};

#endif // MYQGRAPHICSVIEW_H
