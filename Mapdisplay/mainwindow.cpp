#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene=new QGraphicsScene();
    centerChanged=new QTimer(this);

    centerChanged->setInterval(1);
    //updateTimer->setInterval(1);
    bool_tiles.resize(2);
    for(auto &el:bool_tiles){
    el.resize(2);
    }

    check_tiles.resize(3);
    for(auto &el:check_tiles){
    el.resize(3);
    }

    sceneSize=800;
     ui->graphicsView->setScene(scene);

     ui->graphicsView->resize(sceneSize,sceneSize);

     count =0;
     zoom=1;
     tilesamount=pow(2,zoom);
     size=400;

     SceneX=-(tilesamount*size-sceneSize)/2;
     SceneY=-(tilesamount*size-sceneSize)/2;
     ui->graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);

   // showCenter();
connect(centerChanged,&QTimer::timeout,this,&MainWindow::showCenter);
centerChanged->start();
installEventFilter(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMapView(int zoom)
{
 qDebug()<<"The center of View in Scene"<<ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().center());



    QPointF viewTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().center()));
    int x=(viewTopLeft.x()- remain(viewTopLeft.x(),size)-SceneX)/size;
    int y=(viewTopLeft.y()- remain(viewTopLeft.y(),size)-SceneY)/size;

    qreal lon=longitude(x,zoom);
    qreal lat=latitude(y,zoom);

    int new_x=tileX(lon,this->zoom);
    int new_y=tileY(lat,this->zoom);



     count =0;




     for(auto &el:tiles){
         delete el;
     }
        tiles.clear();
     tilesamount=pow(2,this->zoom);
     SceneX=-(tilesamount*size- sceneSize)/2;
     SceneY=-(tilesamount*size- sceneSize)/2;

     sceneTopLeft.setX(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).x()-size);
     sceneTopLeft.setY(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).y()-size);

     sceneBottomRight.setX(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().bottomRight()).x()+size);
     sceneBottomRight.setY(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().bottomRight()).y()+size);


     ui->graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);
     ui->graphicsView->centerOn(SceneX+(new_x)*size+size,SceneY+(new_y)*size+size);



     for(auto &el:check_tiles){
         el.clear();

     }

    check_tiles.resize(3);

    for(auto &el:check_tiles){

    el.resize(3);
    }


    issetted=true;
}


bool MainWindow::intersected(QPointF firstTopLeft, QPointF firstBottomRight, QPointF secondTopLeft, QPointF secondBottomRight)
{
    if(firstTopLeft.x()>secondBottomRight.x()||secondTopLeft.x()>firstBottomRight.x())
        return false;

    if(firstTopLeft.y()>secondBottomRight.y()||secondTopLeft.y()>firstBottomRight.y())
        return false;
    return true;
}

double MainWindow::remain(double number, double divider)
{
    if (number >= 0)
            return number-((int)(number/divider)*divider);


        else
        {
            int quotient = abs(number/divider)+1;
            if ((quotient*divider + number) == divider)
                        return 0;
            return quotient*divider + number;
    }
}



qreal MainWindow::longitude(int x,int z)
{
    return x / (double)(1 << z) * 360.0 - 180;
}

qreal MainWindow::latitude(int y, int z)
{
    double n = M_PI - 2.0 * M_PI * y / (double)(1 << z);
    return (180.0 / M_PI) * atan(0.5 * (exp(n) - exp(-n)));
}

int MainWindow::tileX(qreal lon, int z)
{
    return (int)(floor((lon + 180.0) / 360.0 * (1 << z)));
}

int MainWindow::tileY(qreal lat, int z)
{
    double latrad = lat * M_PI/180.0;
        return (int)(floor((1.0 - asinh(tan(latrad)) / M_PI) / 2.0 * (1 << z)));
}

void MainWindow::removeNonVisibleTiles()
{
     QPointF viewTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()));
    QPointF firstTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()));
     QPointF firstBottomRight(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().bottomRight()));
    foreach (QGraphicsItem* item,scene->items()) {
        if(!intersected(firstTopLeft,firstBottomRight,QPointF(item->x(),item->y()),QPointF(item->x()+size,item->y()+size))){

        scene->removeItem(item);
        int i=(item->x()-SceneX)/size;
        int j=(item->y()-SceneY)/size;

        check_tiles[i%3][j%3]=false;
        auto iter =std::find_if(tiles.begin(),tiles.end(),[&](MapTile*tile){return tile->x()==item->x()&&tile->y()==item->y();});
        tiles.erase(iter);
        }
    }
}








void MainWindow::loadImage(int number,int x,int y)
{


    QImage im;

    bool load=im.loadFromData(static_cast<FileDownloader*>(sender())->downloadedData());
    tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));

    scene->addItem(tiles.back());
    tiles.back()->setPos(QPointF(x*size+SceneX,y*size+SceneY));




}

void MainWindow::showCenter()
{
if(issetted){
    QPointF viewTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()));
    qreal x=viewTopLeft.x()- remain(viewTopLeft.x(),size);
    qreal y=viewTopLeft.y()- remain(viewTopLeft.y(),size);
 qDebug()<<"Vector tiles size"<<tiles.size()<<" and sceneItems size: "<<scene->items().size()<<endl;

    QPointF firstTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()));
    QPointF firstBottomRight(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().bottomRight()));

    removeNonVisibleTiles();

    int k=(x-SceneX)/size;
    int l=(y-SceneY)/size;
    for(int i=k;i<k+3&&i<tilesamount;++i){
        for(int j=l;j<l+3&&j<tilesamount;++j){


            if(intersected(firstTopLeft,firstBottomRight,QPointF(SceneX+i*size,SceneY+j*size),QPointF(SceneX+i*size+size,SceneY+j*size+size)))
             {

                if(!check_tiles[i%3][j%3]){
                //qDebug()<<"The center of View in Scene"<<ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft())<<" And count: "<<count<<endl;
                QUrl OpenStreetMap(api+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");
                 QUrl Yandexmap("https://vec02.maps.yandex.net/tiles?l=map&v=20.09.07-1&x="+QString::number(i)+"&y="+QString::number(j)+"&z="+QString::number(zoom));

                 if(api=="https://tile.openstreetmap.org/")
                    urlpai.setUrl(api+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");
                else if(api=="https://vec02.maps.yandex.net/"){
                    urlpai.setUrl("https://vec02.maps.yandex.net/tiles?l=map&v=20.09.07-1&x="+QString::number(i)+"&y="+QString::number(j)+"&z="+QString::number(zoom));
                }


                 check_tiles[i%3][j%3]=true;
                connect(new FileDownloader(urlpai,count,i,j, this),&FileDownloader::downloaded,this,&MainWindow::loadImage);

                ++count;



                }
            }

        }
    }
    ui->textEdit->setText(QString::number(k)+" "+QString::number(l));


}
}






void MainWindow::on_pushButton_clicked()
{
    if(this->zoom<19){
    issetted=false;

    this->changeMapView(this->zoom++);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(this->zoom>1){
    issetted=false;

    this->changeMapView(this->zoom--);
    }
}

void MainWindow::on_actionOpen_Streen_Map_triggered()
{
    if(api!="https://tile.openstreetmap.org/"){
    api="https://tile.openstreetmap.org/";
    this->changeMapView(this->zoom);

    }

}

void MainWindow::on_actionYandex_Map_triggered()
{
    if(api!="https://vec02.maps.yandex.net/"){
    api="https://vec02.maps.yandex.net/";
    this->changeMapView(this->zoom);
    }
}




void MainWindow::on_pushButton_3_clicked()
{

}
