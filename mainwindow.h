#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

#include "transactionform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void PrepareTable();
    void CreateForm();
    void AddTransactions(int type);

public slots:
    void ShowTransactions();
    void IncomeTransactions();
    void ExpenseTransactions();

private:
    Ui::MainWindow *ui;
    QSqlDatabase Db;
    QString DataBaseName = "cmm.db";
};

#endif // MAINWINDOW_H
