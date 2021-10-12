#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>
#include <QMainWindow>
#include <QtSql/QtSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QListWidget>
#include <string>
#include <stdio.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void requestVermogen();
    void connectDB();
    void showImage();

private slots:
    void on_lwMerk_itemClicked(QListWidgetItem *item);

    void on_lwModel_itemClicked(QListWidgetItem *item);

    void on_btPK_clicked();

    void on_btKW_clicked();

private:
    Ui::MainWindow *ui;
    QString chosenModel;
    QString chosenMerk;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
