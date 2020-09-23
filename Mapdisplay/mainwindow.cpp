#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDebug>

/*WORKING VERSION*/

/*
In main conctructor I setup initial state
Calculate size and position of scene

*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    graphicsView=new MapView(this);
    scene=new MapScene(this);
    centerChanged=new QTimer(this);
    centerChanged->setInterval(1);
    scene->setFocusOnTouch(true);
    ui->verticalLayout->addWidget(graphicsView);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    graphicsView->setMouseTracking(true);


    popUp = new PopUp();

    this->setFocus();
    sceneSize=512;
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tilesInView=9;
    load_tem=false;
    check_tiles.resize(tilesInView);
    for(auto &el:check_tiles){
    el.resize(tilesInView);
    }


    graphicsView->setScene(scene);


   graphicsView->resize(sceneSize,sceneSize);


    //graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->centerOn(size,size);
   zoom=4;
   tilesamount=pow(2,zoom);
   size=256;

   SceneX=-(tilesamount*size-sceneSize)/2;
   SceneY=-(tilesamount*size-sceneSize)/2;
   graphicsView->setSceneRect( SceneX-sceneSize,SceneY-sceneSize,tilesamount*size+2*sceneSize,tilesamount*size+2*sceneSize);
    //graphicsView->setSceneRect( SceneX,SceneY,tilesamount*size,tilesamount*size);



connect(centerChanged,&QTimer::timeout,this,&MainWindow::showCenter);
connect(scene,&MapScene::increaseZoom,this,&MainWindow::increaseZoomByDoubleClick);
connect(scene,&MapScene::signalTargetCoordinate,this,&MainWindow::slotTargetCoordinate);
connect(scene,&MapScene::showPopUp,this,&MainWindow::showPopUp);
connect(graphicsView,&MapView::changeZoomWheel,this,&MainWindow::changeZoomByWheel);

centerChanged->start();
installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
This function is used to recalculate scene's position, scene's size with new zoom and amount of tiles
Also this function calculate focus point  according to view's center with previous zoom
*/
void MainWindow::changeMapView(int zoom, QString zoomtype, QPointF viewTopCenter)
{

     QPointF prevCenter(graphicsView->mapToScene(graphicsView->viewport()->rect().center()));

     tilesamount=pow(2,this->zoom);
     SceneX=-(tilesamount*size- sceneSize)/2;
     SceneY=-(tilesamount*size- sceneSize)/2;




   graphicsView->setSceneRect( SceneX-sceneSize,SceneY-sceneSize,tilesamount*size+2*sceneSize,tilesamount*size+2*sceneSize);
          qDebug()<<QPointF(scene->width(),scene->height())<<endl;
     if(zoomtype=="increase"){
         graphicsView->centerOn(size+2*(viewTopCenter.x()-size),size+2*(viewTopCenter.y()-size));
     }

     else if(zoomtype=="decrease")
         graphicsView->centerOn(size+0.5*(viewTopCenter.x()-size),size+0.5*(viewTopCenter.y()-size));

     else if(zoomtype=="mouseincrease")
         graphicsView->centerOn(QPointF(size+2*(viewTopCenter.x()-size)+(prevCenter.x()-viewTopCenter.x()),size+2*(viewTopCenter.y()-size)+(prevCenter.y()-viewTopCenter.y())));

     else if(zoomtype=="mousedecrease")
         graphicsView->centerOn(QPointF(size+0.5*(viewTopCenter.x()-size)+(prevCenter.x()-viewTopCenter.x()),size+0.5*(viewTopCenter.y()-size)+(prevCenter.y()-viewTopCenter.y())));

     else if(zoomtype=="yandex"){
         api="https://vec02.maps.yandex.net/";
         int x=(viewTopCenter.x()- remain(viewTopCenter.x(),size)-SceneX)/size;
         int y=(viewTopCenter.y()- remain(viewTopCenter.y(),size)-SceneY)/size;

         double lon=longitude(x,this->zoom,remain(viewTopCenter.x(),size));
         double lat=latitude(y,this->zoom,remain(viewTopCenter.y(),size));

         int new_x=TwoGis_tileX(lon,this->zoom).first;
         int new_y=TwoGis_tileY(lat,this->zoom).first;

         graphicsView->centerOn(SceneX+ (new_x)*256+TwoGis_tileX(lon,this->zoom).second-2,SceneY+(new_y)*256+TwoGis_tileY(lat,this->zoom).second-2);

     }

     else if(zoomtype=="osm"){

         if(api=="https://vec02.maps.yandex.net/"){
             int x=(viewTopCenter.x()- remain(viewTopCenter.x(),size)-SceneX)/size;
             int y=(viewTopCenter.y()- remain(viewTopCenter.y(),size)-SceneY)/size;

             double lon=TwoGis_longitude(x,this->zoom,remain(viewTopCenter.x(),size));
             double lat=TwoGis_latitude(y,this->zoom,remain(viewTopCenter.y(),size));

             int new_x=tileX(lon,this->zoom).first;
             int new_y=tileY(lat,this->zoom).first;

             graphicsView->centerOn(SceneX+ (new_x)*256+tileX(lon,this->zoom).second+2,SceneY+(new_y)*256+tileY(lat,this->zoom).second+2);

         }
         api="https://tile.openstreetmap.org/";

     }

     else if(zoomtype=="2gis"){

        if(api=="https://vec02.maps.yandex.net/"){
            int x=(viewTopCenter.x()- remain(viewTopCenter.x(),size)-SceneX)/size;
            int y=(viewTopCenter.y()- remain(viewTopCenter.y(),size)-SceneY)/size;

            double lon=TwoGis_longitude(x,this->zoom,remain(viewTopCenter.x(),size));
            double lat=TwoGis_latitude(y,this->zoom,remain(viewTopCenter.y(),size));

            int new_x=tileX(lon,this->zoom).first;
            int new_y=tileY(lat,this->zoom).first;

            graphicsView->centerOn(SceneX+ (new_x)*256+tileX(lon,this->zoom).second+2,SceneY+(new_y)*256+tileY(lat,this->zoom).second+2);

        }
         api="https://tile2.maps.2gis.com/";

     }

     for(auto &el:check_tiles){
        el.clear();
     }

     check_tiles.resize(tilesInView);

     for(auto &el:check_tiles){
        el.resize(tilesInView);
     }

        load_tem=true;
     issetted=true;
     for(auto &el:tiles){
        delete el;
     }

     tiles.clear();
}

/*
Check if two rectangles intersect
Use for check if our view rectangle is intersected with rectangles where tiles have to be
*/
bool MainWindow::intersected(QPointF firstTopLeft, QPointF firstBottomRight, QPointF secondTopLeft, QPointF secondBottomRight)
{
    if(firstTopLeft.x()>secondBottomRight.x()||secondTopLeft.x()>firstBottomRight.x())
        return false;

    if(firstTopLeft.y()>secondBottomRight.y()||secondTopLeft.y()>firstBottomRight.y())
        return false;
    return true;
}

/*
Calculating remainder of the division
I write this function instead of using "%" operator because this operator returns negative remainder if dividend is negative.
*/
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



double MainWindow::longitude(int x,int z,double picX)
{
    double bm0= 256 * pow(2,z)/2;
      double flatX = x * 256 + picX;

     double lonrad=( M_PI * (flatX - bm0) / bm0);




      double lon = 180*lonrad/M_PI;
      return lon;
}

double MainWindow::latitude(int y, int z,double picY)
{
    double bm0= 256 * pow(2,z)/2;
      double flatY = y * 256 + picY;

      double c1= 2 * M_PI * (bm0 - flatY) / bm0;
      double latrad = asin((exp(c1) - 1) / (exp(c1) + 1));

     double lat= 180*latrad/M_PI;

     return lat;

}

double MainWindow::TwoGis_longitude(int x, int z,double picX)
{

    double a=  6378137.0;

    double flatX=x * 256+picX;

      double mercX =(flatX * pow(2,23 - z)) / 53.5865938 - 20037508.342789;
      double lon = ( mercX / a * 180.0 / M_PI);
      return lon;
}

double MainWindow::TwoGis_latitude(int y, int z,double picY)
{
    double a=  6378137.0;
    double c1= 0.00335655146887969;
    double c2 =0.00000657187271079536;
    double c3 =0.00000001764564338702;
    double c4 =0.00000000005328478445;


     double flatY=y * 256+picY;

       double mercY = ( 20037508.342789 - (flatY * pow(2,23 - z)) / 53.5865938 );
      double g =( M_PI/2 - 2 * atan(1.0 / exp(mercY / a)) );
      double f = ( g + c1 * sin(2 * g) + c2 * sin(4 * g) + c3 * sin(6 * g) + c4 * sin(8 * g));

      double lat =( f * 180.0 / M_PI);

      return lat;

}

std::pair<int,double> MainWindow::tileX(double lon, int z)
{

      double lonrad=lon*M_PI/180.0;

      double bm0 = 256 * pow(2,z)/2;
      int flatX = int(bm0 * (1 + lonrad/M_PI));

      int tilesX= int(flatX/256);

      double picX =flatX%256;
      return std::make_pair(tilesX,picX);

}

std::pair<int,double> MainWindow::tileY(double lat, int z)
{
    double latrad=lat*M_PI/180.0;


      double bm0 = 256 * pow(2,z)/2;

      int flatY =int(bm0 * (1 - 0.5*log((1+sin(latrad))/(1-sin(latrad)))/M_PI));

      int tilesY=int(flatY/256);

     double picY =flatY%256;
     return std::make_pair(tilesY,picY);
}

std::pair<int,double> MainWindow::TwoGis_tileX(double lon, int z)
{
    double lonrad=lon*M_PI/180.0;
    double a =6378137.0;
     double k= 0.0818191908426;

     int flatX=int((20037508.342789 + a *lonrad) * 53.5865938 / pow(2,23 - z));

     int tilesX= int(flatX/256);

     double picX =flatX % 256;
     return std::make_pair(tilesX,picX);

}

std::pair<int,double> MainWindow::TwoGis_tileY(double lat, int z)
{
    double latrad=lat*M_PI/180.0;
    double a =6378137.0;
     double k= 0.0818191908426;
     double f = tan(M_PI / 4 + latrad / 2)/pow((tan(M_PI / 4 + asin(k * sin(latrad)) / 2)), k);

    int flatY= int((20037508.342789 - a * log(f)) * 53.5865938 / pow(2, 23 - z));

     int tilesY = int(flatY/256);

     double picY = flatY % 256;
     return std::make_pair(tilesY,picY);

}

/*
This function removes tiles that are not in our view
*/
void MainWindow::removeNonVisibleTiles()
{
if(issetted){
     QPointF firstTopLeft(graphicsView->mapToScene(graphicsView->viewport()->rect().topLeft()));
     QPointF firstBottomRight(graphicsView->mapToScene(graphicsView->viewport()->rect().bottomRight()));
     foreach (QGraphicsItem* item,scene->items()) {
        if(!intersected(firstTopLeft,firstBottomRight,QPointF(item->x(),item->y()),QPointF(item->x()+size,item->y()+size))){

        scene->removeItem(item);
        int i=(item->x()-SceneX)/size;
        int j=(item->y()-SceneY)/size;

        check_tiles[i%tilesInView][j%tilesInView]=false;
        auto iter =std::find_if(tiles.begin(),tiles.end(),[&](MapTile*tile){return tile->x()==item->x()&&tile->y()==item->y();});
        tiles.erase(iter);
        }
    }
}
}



void MainWindow::loadImage(int x, int y)
{
    /*if(issetted==false){
        if(load_tem){
        QImage im;
        im.loadFromData(static_cast<FileDownloader*>(sender())->downloadedData());
        //temp_tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));
        temp_tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));
        scene->addItem(temp_tiles.back());
        temp_tiles.back()->setPos(QPointF(x*size+SceneX,y*size+SceneY));
        if(temp_tiles.size()>=16){
            for(auto &el:tiles){
                delete el;
            }
            tiles.clear();
            for(auto &el:temp_tiles){
                tiles.push_back(el);
            }
            temp_tiles.clear();
            issetted=true;
            load_tem=false;
        }

    }
    }
else{*/
    QImage im;
    im.loadFromData(static_cast<FileDownloader*>(sender())->downloadedData());
    //temp_tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));
    tiles.push_back(new MapTile(QPixmap::fromImage(im.scaled(size,size,Qt::IgnoreAspectRatio),Qt::AutoColor)));
    scene->addItem(tiles.back());
    tiles.back()->setPos(QPointF(x*size+SceneX,y*size+SceneY));
    if(tiles.size()>=16){
        /*Remove previous layout*/
    }

}


/*
This function calculate which tiles have to be on view
*/
void MainWindow::showCenter()
{

        QPointF viewTopLeft(graphicsView->mapToScene(graphicsView->viewport()->rect().topLeft()));
        QPointF viewBottomRight(graphicsView->mapToScene(graphicsView->viewport()->rect().bottomRight()));
        QPointF viewCenter(graphicsView->mapToScene(graphicsView->viewport()->rect().center()));

        double x=viewTopLeft.x()- remain(viewTopLeft.x(),size);
        double y=viewTopLeft.y()- remain(viewTopLeft.y(),size);


        double lon_x=viewCenter.x()- remain(viewCenter.x(),size);
        double lat_y=viewCenter.y()- remain(viewCenter.y(),size);





        removeNonVisibleTiles();

        int k=(x-SceneX)/size;
        int l=(y-SceneY)/size;

        int lon_k=(lon_x-SceneX)/size;
        int lat_l=(lat_y-SceneY)/size;
        double map_lon;
        double map_lat;
        if(this->api=="https://vec02.maps.yandex.net/")
        {
            map_lon=TwoGis_longitude(lon_k,zoom,remain(viewCenter.x(),size));
             map_lat=TwoGis_latitude(lat_l,zoom,remain(viewCenter.y(),size));

        }
        else{
         map_lon=longitude(lon_k,zoom,remain(viewCenter.x(),size));
         map_lat=latitude(lat_l,zoom,remain(viewCenter.y(),size));
        }

        for(int i=k;i<k+tilesInView&&i<tilesamount;++i){
            for(int j=l;j<l+tilesInView&&j<tilesamount;++j){

                if(i>=0&&j>=0&& intersected(viewTopLeft,viewBottomRight,QPointF(SceneX+i*size,SceneY+j*size),QPointF(SceneX+i*size+size,SceneY+j*size+size)))
                 {

                    if(!check_tiles[i%tilesInView][j%tilesInView]){


                         if(api=="https://tile.openstreetmap.org/")
                            urlpai.setUrl(api+QString::number(zoom)+"/"+QString::number(i)+"/"+QString::number(j)+".png");

                         else if(api=="https://vec02.maps.yandex.net/")
                            urlpai.setUrl("https://vec02.maps.yandex.net/tiles?l=map&v=20.09.07-1&x="+QString::number(i)+"&y="+QString::number(j)+"&z="+QString::number(zoom));

                         else if(api=="https://tile2.maps.2gis.com/")
                            urlpai.setUrl("http://tile2.maps.2gis.com/tiles?x="+QString::number(i)+"&y="+QString::number(j)+"&z="+QString::number(zoom)+"&v=1&ts=online_sd");

                         check_tiles[i%tilesInView][j%tilesInView]=true;
                         connect(new FileDownloader(urlpai,i,j, this),&FileDownloader::downloaded,this,&MainWindow::loadImage);


                    }
                }

            }
        }
        ui->lineEdit->setText(QString::number(this->zoom)+"/"+QString::number(map_lat)+"/"+QString::number(map_lon));




}


void MainWindow::on_pushButton_clicked()
{
    if(this->zoom<19){
    issetted=false;

    this->changeMapView(this->zoom++,"increase",graphicsView->mapToScene(graphicsView->viewport()->rect().center()));
    }

}

void MainWindow::on_pushButton_2_clicked()
{
    if(this->zoom>1){
    issetted=false;

    this->changeMapView(this->zoom--,"decrease",graphicsView->mapToScene(graphicsView->viewport()->rect().center()));
    }
}

void MainWindow::on_actionOpen_Streen_Map_triggered()
{
    if(api!="https://tile.openstreetmap.org/"){

    this->changeMapView(this->zoom,"osm",graphicsView->mapToScene(graphicsView->viewport()->rect().center()));

    }

}

void MainWindow::on_actionYandex_Map_triggered()
{
    if(api!="https://vec02.maps.yandex.net/"){

    this->changeMapView(this->zoom,"yandex",graphicsView->mapToScene(graphicsView->viewport()->rect().center()));
    }
}

void MainWindow::on_action2Gis_triggered()
{
    if(api!="https://tile2.maps.2gis.com/"){

    this->changeMapView(this->zoom,"2gis",graphicsView->mapToScene(graphicsView->viewport()->rect().center()));
    }
}

void MainWindow::showPopUp(QPointF clickPoint)
{
    int x=(clickPoint.x()- remain(clickPoint.x(),size)-SceneX)/size;
    int y=(clickPoint.y()- remain(clickPoint.y(),size)-SceneY)/size;
    double map_lon;
    double map_lat;
    if(this->api=="https://vec02.maps.yandex.net/")
    {
        map_lon=TwoGis_longitude(x,zoom,remain(clickPoint.x(),size));
         map_lat=TwoGis_latitude(y,zoom,remain(clickPoint.y(),size));

    }
    else{
     map_lon=longitude(x,zoom,remain(clickPoint.x(),size));
     map_lat=latitude(y,zoom,remain(clickPoint.y(),size));
    }
    popUp->setPopupText(QString::number(map_lat)+"    "+QString::number(map_lon));
     popUp->show();
}

void MainWindow::increaseZoomByDoubleClick(QPointF clickPoint)
{



    if(this->zoom<19){
    issetted=false;

    this->changeMapView(this->zoom++,"mouseincrease",clickPoint);
    }
}

void MainWindow::changeZoomByWheel(bool zoom, QPointF wheelPoint)
{
    if(zoom){
        if(this->zoom<19){
        issetted=false;

        this->changeMapView(this->zoom++,"mouseincrease",QPointF(wheelPoint.x(),wheelPoint.y()));
        }

    }
    else{
        if(this->zoom>0){
        issetted=false;

        this->changeMapView(this->zoom--,"mousedecrease",QPointF(wheelPoint.x(),wheelPoint.y()));
        }

    }
}

void MainWindow::slotTargetCoordinate(QPointF target)
{
    double lon_x=target.x()- remain(target.x(),size);
    double lat_y=target.y()- remain(target.y(),size);

    int lon_k=(lon_x-SceneX)/size;
    int lat_l=(lat_y-SceneY)/size;
    double map_lon;
    double map_lat;
    if(this->api=="https://vec02.maps.yandex.net/")
    {
        map_lon=TwoGis_longitude(lon_k,zoom,remain(target.x(),size));
         map_lat=TwoGis_latitude(lat_l,zoom,remain(target.y(),size));

    }
    else{
        map_lon=longitude(lon_k,zoom,remain(target.x(),size));
        map_lat=latitude(lat_l,zoom,remain(target.y(),size));
    }
    ui->lineEdit_2->setText(QString::number(map_lat)+"    "+QString::number(map_lon));
}







