#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");

    ui->lLocatie->hide();
    ui->lModel->hide();
    ui->lVermogen->hide();
    ui->gbEenheid->hide();
    ui->lwVermogen->hide();
    ui->lwLocatie->hide();
    ui->lwModel->hide();
    MainWindow::connectDB();
    QPixmap pixmap("C:/Users/timla/Documents/GitHub/EindopdrachtDatabases/EindopdrachtDB/NFS images/Need_For_Speed_logo.png");
    ui->lHomePlaatje->setPixmap(pixmap);

     if(db.open()){
         cout << "Database connected" << endl;
         QSqlQuery query;
         if(query.exec("SELECT * FROM showMerk")){
             while(query.next()){
                 ui->lwMerk->addItem(query.value(0).toString());
             }
         }
    }

     else{
        cout << "Database not connected" << endl;
     }
    db.close();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectDB(){
    db = QSqlDatabase::addDatabase("QMYSQL"); // default constructor
         db.setHostName("localhost");
         db.setPort(3306); // voor de zekerheid
         db.setDatabaseName("nfs");
         db.setUserName("root");
         db.setPassword("");
}

void MainWindow::on_lwMerk_itemClicked(QListWidgetItem *item)
{
    ui->lHomePlaatje->hide();
    ui->lwModel->clear();
    ui->lwModel->show();
    ui->lwVermogen->hide();
    ui->lwVermogen->clear();
    ui->gbEenheid->hide();
    ui->lwLocatie->show();
    ui->lwLocatie->clear();
    ui->lModel->show();
    ui->lLocatie->show();
    ui->lVermogen->hide();
    chosenMerk = item->text();
    showImage();

   if(db.open()){
       cout << "Database connected" << endl;

       QSqlQuery queryModel;
       queryModel.prepare("SELECT Automodellen FROM showModel WHERE showModel.Merk = :merktosend");
       queryModel.bindValue(":merktosend", chosenMerk);

       if(queryModel.exec()){
           while(queryModel.next()){
               ui->lwModel->addItem(queryModel.value(0).toString());
           }
       }
  }
   db.close();
   if(db.open()){
       QSqlQuery queryLocatie;
       queryLocatie.prepare("SELECT locatiezin FROM showLocatie WHERE showLocatie.merk = :merktosend");
       queryLocatie.bindValue(":merktosend", chosenMerk);

       if(queryLocatie.exec()){
           while(queryLocatie.next()){
               ui->lwLocatie->addItem(queryLocatie.value(0).toString());
           }
       }
   }

}

void MainWindow::requestVermogen(){
    ui->lwVermogen->clear();
    ui->lVermogen->show();
     if(db.open()){
         QSqlQuery queryVermogen;
          queryVermogen.prepare("SELECT PK FROM showVermogenModel WHERE Automodellen = :modeltosend");
          queryVermogen.bindValue(":modeltosend", chosenModel);

     if(ui->btPK->isChecked()){
         if(queryVermogen.exec()){
             while(queryVermogen.next()){
                 ui->lwVermogen->addItem(queryVermogen.value(0).toString());
             }
         }
     }
     else if(ui->btKW->isChecked()){
         if(queryVermogen.exec()){
             while(queryVermogen.next()){
                 ui->lwVermogen->addItem(QString::number(queryVermogen.value(0).toInt()/1.36));
             }
         }
     }
     }
     db.close();
}

void MainWindow::on_lwModel_itemClicked(QListWidgetItem *item)
{
    ui->gbEenheid->show();
    ui->lwVermogen->show();
    chosenModel = item->text();
    requestVermogen();
}


void MainWindow::on_btPK_clicked()
{
    requestVermogen();
}


void MainWindow::on_btKW_clicked()
{
    requestVermogen();
}

void MainWindow::showImage(){
    std::string image;

    if(db.open()){
        QSqlQuery queryImage;
        queryImage.prepare("SELECT Logo FROM tblmerk WHERE merk = :merktosend");
        queryImage.bindValue(":merktosend", chosenMerk);
        if(queryImage.exec()){
            while(queryImage.next()){
                 image = queryImage.value(0).toString().toStdString();
            }
        }
    }

    std::string path = "C:/Users/timla/Documents/GitHub/EindopdrachtDatabases/EindopdrachtDB/NFS images/";
    std::string fullpath = path + image;

    QPixmap pixmap(fullpath.c_str());
    ui->lMerkPlaatje->setPixmap(pixmap);
}
