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
    connect(ui->income, SIGNAL(triggered()), this, SLOT(IncomeTransactions()));
    connect(ui->expense, SIGNAL(triggered()), this, SLOT(ExpenseTransactions()));
    connect(ui->report, SIGNAL(triggered()), this, SLOT(CreateReport()));
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
    QVector<QVector<QString>> result;
    GetAllTransactions(result);
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

void MainWindow::CreateForm() {
}

void MainWindow::AddTransactions(int type) {
    Transaction transaction;
    transaction.Type = type == 1 ? "+" : "-";
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

void MainWindow::IncomeTransactions() {
    this->AddTransactions(1);
}

void MainWindow::ExpenseTransactions() {
    this->AddTransactions(0);
}

void MainWindow::CreateReport() {
    QLineSeries* seriesIncome = new QLineSeries();
    QLineSeries* seriesExpenses = new QLineSeries();
    QVector<QVector<QString>> result;
    GetAllTransactions(result);
    for (const auto& row : result) {
        if (row[2] == "+")
            seriesIncome->append(Date2Utc(row[3]), row[1].toInt());
        else
            seriesExpenses->append(Date2Utc(row[3]), row[1].toInt());
    }
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
