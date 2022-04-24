#include "invoiceviewer.h"
#include "ui_invoiceviewer.h"
#include "mainwindow.h"
#include <iostream>

int orderID;
QWidget *pic;

invoiceViewer::invoiceViewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::invoiceViewer)
{
    ui->setupUi(this);

    pic = new QWidget(this);
}

invoiceViewer::~invoiceViewer()
{
    delete ui;
}

void invoiceViewer::setOrderID(int id){
    orderID = id;
    viewInvoice();
}

void invoiceViewer::viewInvoice(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT invoice FROM Orders WHERE orderid = :orderid");

    qry.bindValue(":orderid", orderID);

    if(!qry.exec()){
        qDebug()<<"Error getting invoice from database!";
    }
    qry.first();
    QByteArray outByteArray = qry.value(0).toByteArray();
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(outByteArray);

    ui->invoiceImageLabel->setPixmap(outPixmap);
    ui->invoiceImageLabel->show();
}
