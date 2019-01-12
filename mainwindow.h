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
#include <QtGui>
#include <QtCharts>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QDateTime>

#include "transactionform.h"

using namespace QtCharts;

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
    void PrepareTable(QStringList headers, size_t rows);
    void GetAllTransactions(QVector<QVector<QString>>& transactions);
    void GetAllArrears(QVector<QVector<QString>>& arrears, QString type);
    qint64 Date2Utc(const QString& data);

public slots:
    void ShowTransactions();
    void CreateReport();
    void AddTransactions();
    void ShowDebtor();
    void ShowCreditor();

private:
    Ui::MainWindow *ui;
    QSqlDatabase Db;
    QString DataBaseName = "cmm.db";
};

#endif // MAINWINDOW_H
