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
    QStringList headers = {"ID", "Сумма", "Дата", "Описание"};
    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->verticalHeader()->setVisible(false);
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
            for (size_t j = 0, jt = 0; j < result[i].size(); j++, jt++) {
                if (j == 2) { //if column is type of transactions
                    jt--;
                    continue;
                }
                QTableWidgetItem* item = new QTableWidgetItem(result[i][j]);
                if (j == 1) {
                    item->setTextColor(result[i][j + 1] == "+" ? Qt::green : Qt::red);
                }
                ui->table->setItem(i, jt, item);
            }
        }
    }

}
