#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // default constructor
     db.setHostName("localhost");
     db.setPort(3306); // voor de zekerheid
     db.setDatabaseName("adlkgjakjldrg");
     db.setUserName("root");
     db.setPassword("");

     if(db.open()){
         cout << "Database connected" << endl;
         QSqlQuery query;
         if(query.exec("SELECT * FROM showMerk")){
             while(query.next()){
                 ui->listWidget->addItem(query.value(0).toString());
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

