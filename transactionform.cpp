#include "transactionform.h"

TransactionForm::TransactionForm(Transaction* transaction, QWidget* parent) : Transaction_(transaction)
{
    Form = new QDialog(parent);
    Form->setModal(true);
    valueEdit = new QLineEdit(Form);
    valueEdit->setValidator(new QRegExpValidator(QRegExp("[0-9].*")));
    dataEdit = new QLineEdit(Form);
    dataEdit->setValidator(new QRegExpValidator(QRegExp("([0-9]{2})\\.([0-9]{2})\\.([0-9]{4})")));
    descriptionEdit = new QLineEdit(Form);
    type = new QComboBox(Form);
    type->addItem("Расход");
    type->addItem("Доход");
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Сумма", valueEdit);
    formLayout->addRow("Дата", dataEdit);
    formLayout->addRow("Описание", descriptionEdit);
    formLayout->addRow("Тип", type);
    QVBoxLayout *hbox = new QVBoxLayout();
    ok = new QPushButton(Form);
    ok->setText("OK");
    ok->setFixedWidth(50);
    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(ok);
    hbox->addLayout(formLayout);
    hbox->addLayout(lay);
    Form->setLayout(hbox);
    Form->setWindowTitle(" ");
    Form->show();
    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(OkClicked()));
}

void TransactionForm::OkClicked() {
    if (Transaction_) {
        Transaction_->Data = dataEdit->text();
        Transaction_->Value = valueEdit->text().toInt();
        Transaction_->Description = descriptionEdit->text();
        Transaction_->Type = type->currentText() == "Доход" ? "+" : "-";
        Close = true;
    }
    Form->close();
}

QDialog* TransactionForm::GetForm() {
    return Form;
}

bool TransactionForm::GetClose() {
    return Close;
}

TransactionForm::~TransactionForm() {
    delete ok;
    delete valueEdit;
    delete dataEdit;
    delete descriptionEdit;
}



