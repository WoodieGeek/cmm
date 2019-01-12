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
#include <QTabWidget>


struct Transaction {
    long long Value;
    QString Type;
    QString Data;
    QString Description;
    QString Object;
};

class TransactionForm : public QObject
{
    Q_OBJECT
public:
    TransactionForm(Transaction* transaction, QWidget* parent = nullptr);
    QDialog* GetForm();
    bool GetClose();
    bool GetCredit();
    ~TransactionForm();
private:
    QWidget* GetFinance(QWidget* parent = nullptr);
    QWidget* GetArrears(QWidget* parent = nullptr);
public slots:
    void FinanceOkClicked();
    void ArrearsOkClicked();
private:
    QDialog *Form;
    QTabWidget* TabWidget;
    Transaction* Transaction_;
    QLineEdit* valueEdit;
    QLineEdit* dataEdit;
    QLineEdit* descriptionEdit;
    QLineEdit* ObjectEdit;
    QPushButton* FinanceOk;
    QPushButton* ArrearsOk;
    QComboBox* FinanceType;
    QComboBox* ArrearsType;
    bool Close = false;
    bool Credit = false;
};

#endif // TRANSACTIONFORM_H
