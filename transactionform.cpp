#include "transactionform.h"

TransactionForm::TransactionForm(Transaction* transaction, QWidget* parent) : Transaction_(transaction)
{
    Form = new QDialog(parent);
    Form->setModal(true);
    valueEdit = new QLineEdit(Form);
    dataEdit = new QLineEdit(Form);
    descriptionEdit = new QLineEdit(Form);
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("Сумма", valueEdit);
    formLayout->addRow("Дата", dataEdit);
    formLayout->addRow("Описание", descriptionEdit);
    QVBoxLayout *hbox = new QVBoxLayout();
    ok = new QPushButton(Form);
    ok->setText("OK");
    ok->setFixedWidth(50);
    QHBoxLayout *lay = new QHBoxLayout();
    lay->addWidget(ok);
    hbox->addLayout(formLayout);
    hbox->addLayout(lay);
    Form->setLayout(hbox);
    Form->show();
    QObject::connect(ok, SIGNAL(clicked()), this, SLOT(OkClicked()));
}

void TransactionForm::OkClicked() {
    if (Transaction_) {
        Transaction_->Data = dataEdit->text();
        Transaction_->Value = valueEdit->text().toInt();
        Transaction_->Description = descriptionEdit->text();
    }
    Form->close();
}

QDialog* TransactionForm::getForm() {
    return Form;
}

TransactionForm::~TransactionForm() {
    delete ok;
    delete valueEdit;
    delete dataEdit;
    delete descriptionEdit;

}



