#include "driverprofile.h"
#include "ui_driverprofile.h"
#include "mainwindow.h"
#include <iostream>

int driverID;
QString driverFullname;
QString driverUsername;
QString driverPassword;
QString driverEmail;
QString driverMobileNumber;
QString driverAddress;
QString driverNiNumber;
QString driverDrivingLicenseId;

int driverKey;

driverprofile::driverprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::driverprofile)
{
    ui->setupUi(this);
}

driverprofile::~driverprofile()
{
    delete ui;
}

QString drencryptString(QString str){
    string msg = str.toStdString();

    string encryptedText = msg;

    for (int i = 0; i < msg.size(); i++) {
        if(msg[i] == 32){ // ignore spaces
            continue;
        } else {
            encryptedText[i] += driverKey;
        }
    }

    return QString::fromStdString(encryptedText);
}

QString drdencryptString(QString str){
    string encpMsg = str.toStdString();

    string decryptedText = encpMsg;

    for (int i = 0; i < encpMsg.size(); i++) {
        if(encpMsg[i] == 32){ // ignore spaces
            continue;
        } else {
            decryptedText[i] = encpMsg[i] - driverKey;
        }
    }

    return QString::fromStdString(decryptedText);
}

void driverprofile::setAccountInformation(int key, int _id){
    driverKey = key;

    driverID = _id;

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT * FROM AccountInformation WHERE accountid = :accountid");

    qry.bindValue(":accountid", driverID);

    if(qry.exec()){

        while (qry.next()) {
            driverFullname = qry.value(1).toString();
            driverUsername = qry.value(2).toString();
            driverPassword = qry.value(3).toString();
            driverEmail = qry.value(4).toString();
            driverMobileNumber = qry.value(5).toString();
            driverAddress = qry.value(6).toString();
            driverNiNumber = qry.value(7).toString();
            driverDrivingLicenseId = qry.value(8).toString();
        }

        ui->dProfileSaveChanges->setEnabled(true);
    } else {
        ui->dProfileSaveChanges->setEnabled(false);
    }

    ui->FullnameLineEdit->setText(drdencryptString(driverFullname));
    ui->UsernameLineEdit->setText(drdencryptString(driverUsername));
    ui->PasswordLineEdit->setText(drdencryptString(driverPassword));
    ui->EmailLineEdit->setText(drdencryptString(driverEmail));
    ui->MobileNumberLineEdit->setText(drdencryptString(driverMobileNumber));
    ui->AddressLineEdit->setText(drdencryptString(driverAddress));
    ui->niNumberLineEdit->setText(drdencryptString(driverNiNumber));
    ui->drivingLicenseIdLineEdit->setText(drdencryptString(driverDrivingLicenseId));
}

void driverprofile::on_dProfileSaveChanges_clicked()
{
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("UPDATE AccountInformation SET fullname = :fullname, username = :username, password = :password, email = :email, mobilenumber = :mobilenumber, address = :address, ninumber = :ninumber, drivinglicenseid = :dlicenseid WHERE accountid = :accountid"));

    qry.bindValue(":fullname",drencryptString(ui->FullnameLineEdit->text()));
    qry.bindValue(":username",drencryptString(ui->UsernameLineEdit->text()));
    qry.bindValue(":password",drencryptString(ui->PasswordLineEdit->text()));
    qry.bindValue(":email",drencryptString(ui->EmailLineEdit->text()));
    qry.bindValue(":mobilenumber",drencryptString(ui->MobileNumberLineEdit->text()));
    qry.bindValue(":address",drencryptString(ui->AddressLineEdit->text()));
    qry.bindValue(":accountid",driverID);
    qry.bindValue(":ninumber", drencryptString(ui->niNumberLineEdit->text()));
    qry.bindValue(":dlicenseid", drencryptString(ui->drivingLicenseIdLineEdit->text()));

    if(qry.exec()){
        QMessageBox::information(this,"Success","Changes saved!");
    } else {
        QMessageBox::warning(this,"Error D01","Changes could not be saved!");
    }

    this->reject();
}
