#include "feedback.h"
#include "ui_feedback.h"
#include "mainwindow.h"

int currentOrderID;

feedback::feedback(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::feedback)
{
    ui->setupUi(this);
}

feedback::~feedback()
{
    delete ui;
}

void feedback::setCurrentOrderID(int id){
    currentOrderID = id;
}

void feedback::on_driverSubmitButton_clicked()
{
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("UPDATE Orders SET feedback = :feedback WHERE orderid = :orderid"));

    qry.bindValue(":feedback", ui->driverFeedbackTextEdit->toPlainText());
    qry.bindValue(":orderid", currentOrderID);

    if(!qry.exec()){
        qDebug() << "Failed to write feedback";
        this->reject();
    } else {
        QMessageBox::information(this, "Order Request", "Order delivered!");
        this->reject();
    }
}
