#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;"); // Set background color of window to white

    // Hide all items that should be hidden at initial view of the window

    ui->lLocatie->hide();
    ui->lModel->hide();
    ui->lVermogen->hide();
    ui->gbEenheid->hide();
    ui->lwVermogen->hide();
    ui->lwLocatie->hide();
    ui->lwModel->hide();

    connectDB(); // Connect to the database

    QPixmap pixmap("C:/Users/timla/Documents/GitHub/EindopdrachtDatabases/EindopdrachtDB/NFS images/Need_For_Speed_logo.png"); // File path to 'home' picture
    ui->lHomePlaatje->setPixmap(pixmap); // Set the image to pixmap of label

     if(db.open()){ // Open connection to the database
         cout << "Database connected" << endl;

         QSqlQuery query;
         if(query.exec("SELECT * FROM showMerk")){ // Show all brands

             while(query.next()){ // While the query has a next result, add it to the list in the window
                 ui->lwMerk->addItem(query.value(0).toString());
             }
         }
       db.close(); // Close the database connection
    }
     else{
        cout << "Database not connected" << endl;
     }




}

MainWindow::~MainWindow()
{
    delete ui;
}

// Connects to database with credentials
void MainWindow::connectDB(){

    db = QSqlDatabase::addDatabase("QMYSQL");
         db.setHostName("localhost");
         db.setPort(3306);
         db.setDatabaseName("nfs");
         db.setUserName("root");
         db.setPassword("");
}

void MainWindow::on_lwMerk_itemClicked(QListWidgetItem *item)
{
    ui->lHomePlaatje->hide();
    ui->lwVermogen->hide();
    ui->gbEenheid->hide();
    ui->lVermogen->hide();

    ui->lwModel->show();
    ui->lwLocatie->show();
    ui->lModel->show();
    ui->lLocatie->show();

    ui->lwVermogen->clear();
    ui->lwModel->clear();
    ui->lwLocatie->clear();

    chosenMerk = item->text(); // Set the current brand to selected list item

    showImage(); // Show current brand image

   if(db.open()){
       cout << "Database connected" << endl;

       QSqlQuery queryModel;
       queryModel.prepare("SELECT Automodellen FROM showModel WHERE showModel.Merk = :merktosend"); // Prepare query with variable
       queryModel.bindValue(":merktosend", chosenMerk); // Add chosen brand to the query

       if(queryModel.exec()){ // execute the query
           while(queryModel.next()){
               ui->lwModel->addItem(queryModel.value(0).toString());
           }
       }
       db.close();
  }

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
                 ui->lwVermogen->addItem(QString::number((round(queryVermogen.value(0).toInt()/1.36))));
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
