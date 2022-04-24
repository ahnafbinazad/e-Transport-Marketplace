#include "driverprof.h"
#include "ui_driverprof.h"
#include "mainwindow.h"
#include <iostream>

int customerID;
QString fullname;
QString username;
QString password;
QString email;
QString mobileNumber;
QString address;

int skey;

driverprof::driverprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::customerprofile)
{
    ui->setupUi(this);
}

driverprof::~customerprofile()
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
            encryptedText[i] += skey;
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
            decryptedText[i] = encpMsg[i] - skey;
        }
    }

    return QString::fromStdString(decryptedText);
}

void driverprof::setAccountInformation(int key, int _id){
    skey = key;

    customerID = _id;

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    //cout << customerID << endl;

    qry.prepare("SELECT * FROM AccountInformation WHERE accountid = :accountid");

    qry.bindValue(":accountid", customerID);

    if(qry.exec()){
        //cout << "Query success!" << endl;

        while (qry.next()) {
            fullname = qry.value(1).toString();
            username = qry.value(2).toString();
            password = qry.value(3).toString();
            email = qry.value(4).toString();
            mobileNumber = qry.value(5).toString();
            address = qry.value(6).toString();

            //cout << fullname.toStdString() << endl;
        }

        ui->pushButton_2Dialog->setEnabled(true);
    } else {
        //cout << "Query fail!" << endl;

        ui->pushButton_2Dialog->setEnabled(false);
    }

    ui->FullnameLineEdit->setText(dencryptString(fullname));
    ui->UsernameLineEdit->setText(dencryptString(username));
    ui->PasswordLineEdit->setText(dencryptString(password));
    ui->EmailLineEdit->setText(dencryptString(email));
    ui->MobileNumberLineEdit->setText(dencryptString(mobileNumber));
    ui->AddressLineEdit->setText(dencryptString(address));
}

void customerprofile::on_pushButton_2Dialog_clicked()
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

    qry.exec();

    this->reject();
}
