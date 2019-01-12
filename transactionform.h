#ifndef TRANSACTIONFORM_H
#define TRANSACTIONFORM_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDebug>
#include <QRegExpValidator>
#include <QComboBox>


struct Transaction {
    long long Value;
    QString Type;
    QString Data;
    QString Description;
};

class TransactionForm : public QObject
{
    Q_OBJECT
public:
    TransactionForm(Transaction* transaction, QWidget* parentt = nullptr);
    QDialog* GetForm();
    bool GetClose();
    ~TransactionForm();
public slots:
    void OkClicked();
private:
    QDialog *Form;
    Transaction* Transaction_;
    QLineEdit* valueEdit;
    QLineEdit* dataEdit;
    QLineEdit* descriptionEdit;
    QPushButton* ok;
    QComboBox* type;

    bool Close = false;
};

#endif // TRANSACTIONFORM_H
