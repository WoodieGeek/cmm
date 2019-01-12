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
    connect(ui->operations, SIGNAL(triggered()), this, SLOT(ShowTransactions()));
    connect(ui->report, SIGNAL(triggered()), this, SLOT(CreateReport()));
    connect(ui->transaction, SIGNAL(triggered()), this, SLOT(AddTransactions()));
    connect(ui->debtor, SIGNAL(triggered()), this, SLOT(ShowDebtor()));
    connect(ui->creditor, SIGNAL(triggered()), this, SLOT(ShowCreditor()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PrepareTable(QStringList headers, size_t rows) {
    ui->table->clear();
    ui->table->setRowCount(rows);
    ui->table->setColumnCount(headers.size());
    ui->table->setHorizontalHeaderLabels(headers);
    ui->table->verticalHeader()->setVisible(false);
    ui->table->showGrid();
}

void MainWindow::ShowTransactions() {
    QVector<QVector<QString>> result;
    GetAllTransactions(result);
    PrepareTable({"ID", "Сумма", "Дата", "Описание"}, result.size());
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


void MainWindow::AddTransactions() {
    Transaction transaction;
    TransactionForm* form = new TransactionForm(&transaction, this);
    form->GetForm()->exec();
    if (form->GetClose()) {
        QSqlQuery* query = new QSqlQuery(Db);
        query->exec("INSERT INTO finance(value, type, data, description) VALUES(" +
                    QString::number(transaction.Value) + ", '" + transaction.Type +
                    "', '" +transaction.Data + "', '" + transaction.Description + "')" );
    }
    delete form;
}

void MainWindow::CreateReport() {
    QLineSeries* seriesIncome = new QLineSeries();
    QLineSeries* seriesExpenses = new QLineSeries();
    QVector<QVector<QString>> result;
    GetAllTransactions(result);
    QVector<QPair<int, int>> Incomes, Expanses;
    for (const auto& row : result) {
        if (row[2] == "+")
            Incomes.push_back({Date2Utc(row[3]), row[1].toInt()});
        else
            Expanses.push_back({Date2Utc(row[3]), row[1].toInt()});
    }
    std::sort(Incomes.begin(), Incomes.end());
    std::sort(Expanses.begin(), Expanses.end());
    for (const auto& point : Incomes)
        seriesIncome->append(point.first, point.second);
    for (const auto& point : Expanses)
        seriesExpenses->append(point.first, point.second);
    seriesExpenses->setColor(Qt::red);
    seriesIncome->setColor(Qt::green);
    seriesExpenses->setName("Расходы");
    seriesIncome->setName("Доходы");
    QChart* chart = new QChart();
    chart->addSeries(seriesIncome);
    chart->addSeries(seriesExpenses);
    chart->createDefaultAxes();
    chart->axisX()->setLabelsVisible(false);
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    this->x();
    chartView->setGeometry(chartView->x(), chartView->y(), 500, 500);
    chartView->setGeometry(this->x(), this->y(),this->childrenRect().width(), this->childrenRect().height());
    chartView->show();
}

void MainWindow::GetAllTransactions(QVector<QVector<QString>>& transactions) {
    QSqlQuery* query = new QSqlQuery(Db);
    if (query->exec("SELECT * FROM finance")) {
        while (query->next()) {
            QVector<QString> row;
            for (size_t i = 0; i < 5; i++) {
                row.push_back(query->value(i).toString());
            }
            transactions.push_back(std::move(row));
        }
    }
}


qint64 MainWindow::Date2Utc(const QString& date) {
    QDateTime dateTime = QDateTime::fromString(date, "dd.MM.yyyy");
    const auto& date_ = dateTime.date();
    return  date_.day() + date_.month() * 100 + date_.year() * 10000;
}

void MainWindow::GetAllArrears(QVector<QVector<QString>>& arrears, QString type) {
    QSqlQuery* query = new QSqlQuery(Db);
    if (query->exec("SELECT id, value, object FROM arrears WHERE type == '" + type + "'")) {
        while (query->next()) {
            QVector<QString> row;
            for (size_t i = 0; i < 4; i++)
                row.push_back(query->value(i).toString());
            arrears.push_back(std::move(row));
        }
    }
}

void MainWindow::ShowDebtor() {
    QVector<QVector<QString>> result;
    GetAllArrears(result, "+");
    PrepareTable({"ID", "Сумма", "Должник"}, result.size());
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            ui->table->setItem(i, j, new QTableWidgetItem(result[i][j]));
        }
    }
}

void MainWindow::ShowCreditor() {
    QVector<QVector<QString>> result;
    GetAllArrears(result, "-");
    PrepareTable({"ID", "Сумма", "Кредитор"}, result.size());
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            ui->table->setItem(i, j, new QTableWidgetItem(result[i][j]));
        }
    }
}





