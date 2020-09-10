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
    bool_tiles.resize(2);
    for(auto &el:bool_tiles){
    el.resize(2);
    }


     ui->graphicsView->setScene(scene);
     ui->graphicsView->centerOn(400,400);

     count =0;
     zoom=1;
     tilesamount=pow(2,zoom);
     size=400;
     SceneX=-(tilesamount*size-800)/2;
     SceneY=-(tilesamount*size-800)/2;
     ui->graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);
    /* for(int i=0;i<tilesamount;++i){
        for(int j=0;j<tilesamount;++j){
            if(bool_tiles[i][j]==false){
                bool_tiles[i][j]=true;
            QUrl imUrl("https://tile.openstreetmap.org/"+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");

            vecTiles.push_back( new FileDownloader(imUrl,count,i,j, this));
            connect(vecTiles[count],&FileDownloader::downloaded,this,&MainWindow::loadImage);

            ++count;
            }
        }
     }*/

connect(centerChanged,&QTimer::timeout,this,&MainWindow::showCenter);
centerChanged->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeMapView(int zoom)
{
 qDebug()<<"The center of View in Scene"<<ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().center());

    this->zoom=zoom;
     count =0;



     for(auto &el:vecTiles){
         delete el;
     }
     vecTiles.clear();

     for(auto &el:tiles){
         delete el;
     }
        tiles.clear();
     tilesamount=pow(2,zoom);
     SceneX=-(tilesamount*size-800)/2;
     SceneY=-(tilesamount*size-800)/2;
     ui->graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);

     for(auto &el:bool_tiles){
         el.clear();

     }

     bool_tiles.resize(tilesamount);
     for(auto &el:bool_tiles){
        // el.clear();
     el.resize(tilesamount);
     }
     /*for(int i=0;i<tilesamount;++i){
        for(int j=0;j<tilesamount;++j){

            QUrl imUrl("https://tile.openstreetmap.org/"+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");

            vecTiles.push_back( new FileDownloader(imUrl,count,i,j, this));
            connect(vecTiles.back(),&FileDownloader::downloaded,this,&MainWindow::loadImage);

            ++count;
        }
     }
*/
    issetted=true;
}

void MainWindow::cool_changeMapView(int zoom)
{
    this->zoom=zoom;
     int count =0;


     for(int i=pow(tilesamount,2)-1;i>=0;--i){
         delete vecTiles[i];
         vecTiles.pop_back();

     }

     tilesamount=pow(2,zoom);
     SceneX=-(tilesamount*size-800)/2;
     SceneY=-(tilesamount*size-800)/2;
     ui->graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);

     for(int i=0;i<tilesamount;++i){
         for(int j=0;j<tilesamount;++j){
             if(ui->graphicsView->viewport()->rect().intersects(QRect(SceneX+i*size,SceneY+j*size,400,400)))
             {QUrl imUrl("https://tile.openstreetmap.org/"+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");

                 vecTiles.push_back( new FileDownloader(imUrl,count,i,j, this));
                 connect(vecTiles[count],&FileDownloader::downloaded,this,&MainWindow::loadImage);

                 ++count;
             }

         }

     }
}

bool MainWindow::intersected(QPointF firstTopLeft, QPointF firstBottomRight, QPointF secondTopLeft, QPointF secondBottomRight)
{
    if(firstTopLeft.x()>=secondBottomRight.x()||secondTopLeft.x()>=firstBottomRight.x())
        return false;

    if(firstTopLeft.y()>=secondBottomRight.y()||secondTopLeft.y()>=firstBottomRight.y())
        return false;
    return true;
}

void MainWindow::loadImage(int number,int x,int y)
{


    QImage im;

    bool loaded=im.loadFromData(vecTiles[number]->downloadedData());


    tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));

    scene->addItem(tiles.back());
    tiles.back()->setPos(QPointF(x*size+SceneX,y*size+SceneY));


}

void MainWindow::showCenter()
{

    //label->setText(QString::number(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).x())+" "+ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).y());
   ui->textEdit->setText(QString::number(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).x())+" "+QString::number(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()).y()));
    if(issetted){

        for(int i=0;i<tilesamount;++i){
            for(int j=0;j<tilesamount;++j){
                QRect rect(ui->graphicsView->viewport()->rect().topLeft(),ui->graphicsView->viewport()->rect().bottomRight());
                QPointF firstTopLeft(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft()));
                 QPointF firstBottomRight(ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().bottomRight()));
                //if(rect.intersects(QRect(SceneX+i*size,SceneY+j*size,400,400)))
                if(intersected(firstTopLeft,firstBottomRight,QPointF(SceneX+i*size,SceneY+j*size),QPointF(SceneX+i*size+400,SceneY+j*size+400)))
                 {

                    if(!bool_tiles[i][j]){
                    qDebug()<<"The center of View in Scene"<<ui->graphicsView->mapToScene(ui->graphicsView->viewport()->rect().topLeft())<<" And count: "<<count<<endl;
                    QUrl imUrl("https://tile.openstreetmap.org/"+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");
                    bool_tiles[i][j]=true;
                    vecTiles.push_back( new FileDownloader(imUrl,count,i,j, this));
                    connect(vecTiles.back(),&FileDownloader::downloaded,this,&MainWindow::loadImage);

                    ++count;
                    }
                }

            }

        }
    }
}






void MainWindow::on_pushButton_clicked()
{
    if(this->zoom<19){
    issetted=false;
    this->zoom++;
    this->changeMapView(this->zoom);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(this->zoom>1){
    issetted=false;
    this->zoom--;
    this->changeMapView(this->zoom);
    }
}
