#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void PrepareTable();

public slots:
    void ShowTransactions();

private:
    Ui::MainWindow *ui;
    QSqlDatabase Db;
    QString DataBaseName = "cmm.db";
};

#endif // MAINWINDOW_H
