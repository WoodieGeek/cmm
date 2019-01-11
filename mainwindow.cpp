#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Db = QSqlDatabase::addDatabase("QSQLITE");
    Db.setDatabaseName(DataBaseName);
    if (!Db.open())
        qDebug() << "DataBase dont open";
    connect(ui->transactions, SIGNAL(triggered()), this, SLOT(ShowTransactions()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PrepareTable() {
    QStringList headers = {"ID", "Сумма", "Тип", "Дата", "Описание"};
    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->showGrid();
}

void MainWindow::ShowTransactions() {
    PrepareTable();
    QSqlQuery* query = new QSqlQuery(Db);
    if (query->exec("SELECT * FROM finance")) {
        QVector<QVector<QString>> result;
        while (query->next()) {
            QVector<QString> row;
            for (size_t i = 0; i < 5; i++) {
                row.push_back(query->value(i).toString());
            }
            result.push_back(row);
        }
        ui->table->setRowCount(result.size());
        for (size_t i = 0; i < result.size(); i++) {
            for (size_t j = 0; j < result[i].size(); j++) {
                ui->table->setItem(i, j, new QTableWidgetItem(result[i][j]));
            }
        }
    }

}
