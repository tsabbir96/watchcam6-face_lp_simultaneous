#include "tilerdisplay.h"
#include "ui_tilerdisplay.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QDir>

TilerDisplay::TilerDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TilerDisplay)
{
    ui->setupUi(this);
    grid_show_all_faces = new QGridLayout; // Initialize Grid Layout
    ui->scrollAreaWidgetContents->setLayout(grid_show_all_faces);
    grid_show_uniq_faces = new QGridLayout;
    grid_show_recog_faces = new QGridLayout;
    //        grid_show_unique_lpr = new QGridLayout;



    ui->scrollAreaWidgetContents->setLayout(grid_show_all_faces);
    //    ui->scrollAreaWidgetContents_2->setLayout(flowLayout2);
    ui->scrollAreaWidgetContents_2->setLayout(grid_show_uniq_faces);

    //        ui->scrollAreaWidgetContents_3->setLayout(grid_show_unique_lpr);

    ui->scrollAreaWidgetContents_4->setLayout(grid_show_recog_faces);

    // ui->scrollAreaFaces->verticalScrollBar()->setSliderPosition(label->height());
    ui->tabWidget->setCurrentIndex(1);

    ui->tabWidget->setCurrentIndex(0);
    qDebug()<<"tilerdisplay constructor call";

    car_img_show_dialog_obj = new CarImgShowDialog(this);

}
void TilerDisplay::addFR(QPixmap pic, QString name, int idx)
{


    Face *newFace = new Face();
    newFace->setObject(pic, name, idx);


    if (!pic.isNull()){
        //qDebug()<<"pic is not null"<<" faceList.size() >>"<<faceList.size();
        if (faceList.size() < 25){// Checked size of list is 100 or not!
            //qDebug()<<"facelist less than 100";
            faceList.push_back(newFace); // Added face in Lists
            grid_show_all_faces->addWidget(newFace, row_index, column_index); // load data into grid layout.
            ui->tabWidget->setTabText(0, "Faces");

        }else {
            //qDebug()<<"face list greater than 100";
            delete faceList.at(0); // deallocation done
            faceList.removeFirst(); // remove first element
            faceList.push_back(newFace); // face added into list

            QListIterator<Face*> iter(faceList); // iterator
            iter.toFront(); // iterator start from first
            ui->tabWidget->setTabText(0, "Faces");

            // set face into grid layout which are new come
            for (int i = 0; i < 5; ++i){
                for (int j = 0; j < 5; ++j){
                    // for 100 cell
                    grid_show_all_faces->addWidget(iter.next(), i, j); // load data into grid layout.
                }

            }

        }
        //  Grid index customize
        if(column_index == 5){
            //qDebug()<<"colum index equal 10";
            row_index++;
            column_index = -1;
        }

        if(row_index == 5){
            //qDebug()<<"row equal 10";
            row_index = 0;
        }
        column_index++;
    }

    ui->scrollAreaFaces->verticalScrollBar()->setSliderPosition(ui->scrollAreaFaces->verticalScrollBar()->maximum());
}

void TilerDisplay::only_uniqueFace_show_inTiles(QPixmap main_pic, QPixmap pic, QString name, int idx, QString car_path)
{
    qDebug()<<"only_uniqueFace_show_inTiles of tilerdis..";

    Face *uniqFace = new Face();
    uniqFace->setObject(pic, name, idx);//make a lp widget

    //connect(uniqFace, SIGNAL(emitPicDB(QPixmap)), this, SLOT(justcheck(QPixmap)));

    // Checked Image Null or Not! After getting correct image, hold into list.
    if (!pic.isNull()){
        //qDebug()<<"pic is not null"<<" uniq_faceList.size() >>"<<uniq_faceList.size();
        if (uniq_faceList.size() < 25){// Checked size of list is 100 or not!
            //qDebug()<<"uniq_faceList less than 100";
            uniq_faceList.push_back(uniqFace); // Added face in Lists
            grid_show_uniq_faces->addWidget(uniqFace, row_index_u, column_index_u); // load data into grid layout.
            //            ui->tabWidget->setTabText(1, "LPR(" + QString::number(uniq_faceList.size()) + ")");
            ui->tabWidget->setTabText(1, "LPR");

        }else {
            //qDebug()<<"face list greater than 100";
            delete uniq_faceList.at(0); // deallocation done
            uniq_faceList.removeFirst(); // remove first element
            uniq_faceList.push_back(uniqFace); // face added into list
            //            ui->tabWidget->setTabText(1, " Unmask Faces (" + QString::number(uniq_faceList.size()) + ")");
            ui->tabWidget->setTabText(1, "LPR");

            QListIterator<Face*> iter(uniq_faceList); // iterator
            iter.toFront(); // iterator start from first


            // set face into grid layout which are new come
            for (int i = 0; i < 5; ++i){
                for (int j = 0; j < 5; ++j){
                    // for 100 cell
                    grid_show_uniq_faces->addWidget(iter.next(), i, j); // load data into grid layout.
                }

            }

        }
        //  Grid index customize
        if(column_index_u==  5){
            //qDebug()<<"colum index equal 10";
            row_index_u++;
            column_index_u = -1;
        }

        if(row_index_u == 5){
            //qDebug()<<"row equal 10";
            row_index_u = 0;
        }
        column_index_u++;
    }

    ui->scrollAreaCluster->verticalScrollBar()->setSliderPosition(ui->scrollAreaCluster->verticalScrollBar()->maximum());

}

void TilerDisplay::clearLPwithCarPairVector()
{
    //qDebug()<<"clearLPwithCarPairVector call and before clear vec sz : "<<LPwithCarPairVector.size()<<" and recog lst : "<<recog_faceList.size();

    LPwithCarPairVector.clear();
    for(int t = 0; t<recog_faceList.size(); t++){
        delete recog_faceList.at(t);
    }

    //qDebug()<<"after clear LpwithCarVec sz is : "<<LPwithCarPairVector.size()<<" and after clear recog lst : "<<recog_faceList.size();

}

void TilerDisplay::displayLPCorespondingCarImage(int id)
{
    QString car_path = LPwithCarPairVector.at(id).second;
    QPixmap car_pic;
    car_pic.load(car_path);

    qDebug()<<"displayLPCorespondingCarImage call and car path : "<<car_path;


    QImage image = car_pic.toImage();

    car_img_show_dialog_obj->carImageSetIntoLabel(id, car_path, image);
    car_img_show_dialog_obj->setFixedSize(car_img_show_dialog_obj->size());
    car_img_show_dialog_obj->exec();
}

void TilerDisplay::add_search_lp_To_tiles(QPixmap pic, QString name, int idx, QString car_path)
{
    Face *newFace2 = new Face();
    newFace2->setFace(pic, name, idx, idx);//make a lp widget

    LPwithCarPairVector.push_back(qMakePair(idx, car_path));
    qDebug()<<"add_recog_face_To_tiles call and LPwithCarPairVector size is : "<<LPwithCarPairVector.size();

    connect(newFace2, SIGNAL(emitCluster(int)), this, SLOT(displayLPCorespondingCarImage(int)));

    // Checked Image Null or Not! After getting correct image, hold into list.
    if (!pic.isNull()){

        if (recog_faceList.size() < 100){// Checked size of list is 100 or not!
            qDebug()<<"facelist less than 100";
            recog_faceList.push_back(newFace2); // Added face in Lists
            grid_show_recog_faces->addWidget(newFace2, row_index_r, column_index_r); // load data into grid layout.
//            ui->tabWidget->setTabText(2, " Search LP (" + QString::number(recog_faceList.size()) + ")");
            ui->tabWidget->setTabText(2, "Search LP");

        }else {
            qDebug()<<"face list greater than 100";
            delete recog_faceList.at(0); // deallocation done
            recog_faceList.removeFirst(); // remove first element
            recog_faceList.push_back(newFace2); // face added into list
//            ui->tabWidget->setTabText(2, " Search LP (" + QString::number(recog_faceList.size()) + ")");
            ui->tabWidget->setTabText(2, "Search LP");

            QListIterator<Face*> iter(recog_faceList); // iterator
            iter.toFront(); // iterator start from first

            // set face into grid layout which are new come
            for (int i = 0; i < 10; ++i){
                for (int j = 0; j < 10; ++j){
                    // for 100 cell
                    grid_show_recog_faces->addWidget(iter.next(), i, j); // load data into grid layout.

                }

            }

        }
        //  Grid index customize
        if(column_index_r == 10){
            ////qDebug()<<"colum index equal 10";
            row_index_r++;
            column_index_r = -1;
        }

        if(row_index_r == 10){
            ////qDebug()<<"row equal 10";
            row_index_r = 0;
        }
        column_index_r++;
    }
    ui->scrollArea->verticalScrollBar()->setSliderPosition(ui->scrollArea->verticalScrollBar()->maximum());
}

TilerDisplay::~TilerDisplay()
{
    for(int i=0;i<faceList.size(); i++){
        delete faceList.at(i);
        faceList.removeAt(i);
    }
    for(int j=0; j<uniq_faceList.size(); j++){
        delete uniq_faceList.at(j);
        uniq_faceList.removeAt(j);
    }
    LPwithCarPairVector.clear();

    delete ui;
}
