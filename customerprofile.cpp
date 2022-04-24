#include "customerprofile.h"
#include "ui_customerprofile.h"
#include "mainwindow.h"
#include <iostream>

int customerID;
QString customerFullname;
QString customerUsername;
QString customerPassword;
QString customerEmail;
QString customerMobileNumber;
QString customerAddress;

int customerKey;

customerprofile::customerprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customerprofile)
{
    ui->setupUi(this);
}

customerprofile::~customerprofile()
{
    delete ui;
}

QString encryptString(QString str){
    string msg = str.toStdString();

    string encryptedText = msg;

    for (int i = 0; i < msg.size(); i++) {
        if(msg[i] == 32){ // ignore spaces
            continue;
        } else {
            encryptedText[i] += customerKey;
        }
    }

    return QString::fromStdString(encryptedText);
}

QString dencryptString(QString str){
    string encpMsg = str.toStdString();

    string decryptedText = encpMsg;

    for (int i = 0; i < encpMsg.size(); i++) {
        if(encpMsg[i] == 32){ // ignore spaces
            continue;
        } else {
            decryptedText[i] = encpMsg[i] - customerKey;
        }
    }

    return QString::fromStdString(decryptedText);
}

void customerprofile::setAccountInformation(int key, int _id){
    customerKey = key;

    customerID = _id;

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    //cout << customerID << endl;

    qry.prepare("SELECT * FROM AccountInformation WHERE accountid = :accountid");

    qry.bindValue(":accountid", customerID);

    if(qry.exec()){
        //cout << "Query success!" << endl;

        while (qry.next()) {
            customerFullname = qry.value(1).toString();
            customerUsername = qry.value(2).toString();
            customerPassword = qry.value(3).toString();
            customerEmail = qry.value(4).toString();
            customerMobileNumber = qry.value(5).toString();
            customerAddress = qry.value(6).toString();

            //cout << fullname.toStdString() << endl;
        }

        ui->cProfileSaveChanges->setEnabled(true);
    } else {
        //cout << "Query fail!" << endl;

        ui->cProfileSaveChanges->setEnabled(false);
    }

    ui->FullnameLineEdit->setText(dencryptString(customerFullname));
    ui->UsernameLineEdit->setText(dencryptString(customerUsername));
    ui->PasswordLineEdit->setText(dencryptString(customerPassword));
    ui->EmailLineEdit->setText(dencryptString(customerEmail));
    ui->MobileNumberLineEdit->setText(dencryptString(customerMobileNumber));
    ui->AddressLineEdit->setText(dencryptString(customerAddress));
}

void customerprofile::on_cProfileSaveChanges_clicked()
{
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("UPDATE AccountInformation SET fullname = :fullname, username = :username, password = :password, email = :email, mobilenumber = :mobilenumber, address = :address WHERE accountid = :accountid"));

    qry.bindValue(":fullname",encryptString(ui->FullnameLineEdit->text()));
    qry.bindValue(":username",encryptString(ui->UsernameLineEdit->text()));
    qry.bindValue(":password",encryptString(ui->PasswordLineEdit->text()));
    qry.bindValue(":email",encryptString(ui->EmailLineEdit->text()));
    qry.bindValue(":mobilenumber",encryptString(ui->MobileNumberLineEdit->text()));
    qry.bindValue(":address",encryptString(ui->AddressLineEdit->text()));
    qry.bindValue(":accountid",customerID);

    if(qry.exec()){
        QMessageBox::information(this,"Success","Changes saved!");
    } else {
        QMessageBox::warning(this,"Error C01","Changes could not be saved!");
    }



    this->reject();
}
