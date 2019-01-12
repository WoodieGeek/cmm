#include "transactionform.h"

TransactionForm::TransactionForm(Transaction* transaction, QWidget* parent) : Transaction_(transaction)
{
    Form = new QDialog(parent);
    QHBoxLayout *mainLay = new QHBoxLayout();
    TabWidget = new QTabWidget(Form);
    TabWidget->addTab(this->GetFinance(), "Переводы");
    TabWidget->addTab(this->GetArrears(), "Займы");
    QObject::connect(FinanceOk, SIGNAL(clicked()), this, SLOT(FinanceOkClicked()));
    QObject::connect(ArrearsOk, SIGNAL(clicked()), this, SLOT(ArrearsOkClicked()));
    mainLay->addWidget(TabWidget);
    Form->setLayout(mainLay);
    Form->setWindowTitle(" ");
    Form->show();
}

QWidget* TransactionForm::GetFinance(QWidget* parent) {
    QWidget* result = new QWidget();
    valueEdit = new QLineEdit(Form);
    valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-9].*")));
    dataEdit = new QLineEdit(Form);
    dataEdit->setValidator(new QRegExpValidator(QRegExp("([0-9]{2})\\.([0-9]{2})\\.([0-9]{4})")));
    descriptionEdit = new QLineEdit(Form);
    FinanceType = new QComboBox(Form);
    FinanceType ->addItem("Расход");
    FinanceType ->addItem("Доход");
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Сумма", valueEdit);
    formLayout->addRow("Дата", dataEdit);
    formLayout->addRow("Описание", descriptionEdit);
    formLayout->addRow("Тип", FinanceType);
    QVBoxLayout *hbox = new QVBoxLayout();
    FinanceOk = new QPushButton(Form);
    FinanceOk->setText("OK");
    FinanceOk->setFixedWidth(50);
    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(FinanceOk);
    hbox->addLayout(formLayout);
    hbox->addLayout(lay);
    result->setLayout(hbox);
    return result;
}

QWidget* TransactionForm::GetArrears(QWidget* parent) {
    QWidget* result = new QWidget();
    valueEdit = new QLineEdit(Form);
    valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-9].*")));
    ObjectEdit = new QLineEdit(Form);
    ArrearsType = new QComboBox(Form);
    ArrearsType->addItem("Кредит");
    ArrearsType->addItem("Дать взаймы");
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Сумма", valueEdit);
    formLayout->addRow("Субъект", ObjectEdit);
    formLayout->addRow("Тип", ArrearsType);
    QVBoxLayout *hbox = new QVBoxLayout();
    ArrearsOk = new QPushButton(Form);
    ArrearsOk->setText("OK");
    ArrearsOk->setFixedWidth(50);
    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(ArrearsOk);
    hbox->addLayout(formLayout);
    hbox->addLayout(lay);
    result->setLayout(hbox);
    return result;
}

void TransactionForm::FinanceOkClicked() {
    if (Transaction_) {
        Transaction_->Data = dataEdit->text();
        Transaction_->Value = valueEdit->text().toInt();
        Transaction_->Description = descriptionEdit->text();
        Transaction_->Type = FinanceType->currentText() == "Доход" ? "+" : "-";
        Close = true; } Form->close();
}

void TransactionForm::ArrearsOkClicked() {
    if (Transaction_) {
        Transaction_->Value = valueEdit->text().toInt();
        Transaction_->Object = ObjectEdit->text();
        Transaction_->Type = ArrearsType->currentText() == "Кредит" ? "-" : "+";
        Close = true;
        Credit = true;
    }
    Form->close();
}

QDialog* TransactionForm::GetForm() {
    return Form;
}

bool TransactionForm::GetClose() {
    return Close;
}

bool TransactionForm::GetCredit() {
    return Credit;
}

TransactionForm::~TransactionForm() {
    delete valueEdit;
    delete dataEdit;
    delete descriptionEdit;
}



