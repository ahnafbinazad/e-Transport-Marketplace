#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customerprofile.h"
#include "driverprofile.h"
#include "invoiceviewer.h"
#include "feedback.h"
#include <fstream>
#include <QDebug>
#include <iostream>
#include <QDebug>

using namespace std;

char str[100];

// user
string loggedUsername = "";
int loggedAccountID = -1;
int ctID;
QString ctFullname = "";
QString ctUsername = "";
QString ctPassword = "";
QString ctEmail = "";
QString ctMobileNumber = "";
QString ctAddress = "";

// cargo
float shippingRate = 0.70;
float transportFee = 5.00;
float commissionFee = 0.00;
float totalPrice = 0;
int maxDeliveryNoteLength = 80;
int selectedOrderID;
int companySelectedOrderID;
int customerHistorySelectedOrderID;
int driverCurrentOrderID;

// model
QSqlQueryModel *myOrdersModel = new QSqlQueryModel();
QSqlQueryModel *allOrdersModel = new QSqlQueryModel();
QSqlQueryModel *orderRequestsModel = new QSqlQueryModel();
QSqlQueryModel *companyOrderHistoryModel = new QSqlQueryModel();
QSqlQueryModel *customerOrderHistoryModel = new QSqlQueryModel();
QSqlQueryModel *driverOrderHistoryModel = new QSqlQueryModel();

const QString PREPARING_STRING = "Preparing";
const QString CONFIRMING_STRING = "Confirming";
const QString LOADING_STRING = "Loading";
const QString ONROAD_STRING = "On Road";
const QString DELIVERED_STRING = "Delivered";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    ui->mainStackedWidget->setCurrentWidget(ui->welcomePage);

    ui->usernameLineEdit->setPlaceholderText("Username");
    ui->passwordLineEdit->setPlaceholderText("Password");
    ui->databaseStatusRegisterLabel->setText("<font color='black'>Waiting...</font>");
    ui->databaseStatusLoginLabel->setText("<font color='black'>Waiting...</font>");
    ui->databaseStatusWelcomeLabel->setText("<font color='black'>Waiting...</font>");
    ConnectToDatabase();

    ui->weightComboBox->setItemData(1,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(2,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(3,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(4,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(5,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(6,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(7,0,Qt::UserRole - 1);
    ui->weightComboBox->setItemData(8,0,Qt::UserRole - 1);

    calculateShippingRate(0, 0);

    ui->sourceComboBox->setItemData(1,0,Qt::UserRole - 1);
    ui->destinationComboBox->setItemData(0,0,Qt::UserRole - 1);

    ui->myOrdersTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->myOrdersTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->myOrdersTableView->verticalHeader()->hide();
    ui->myOrdersTableView->setAlternatingRowColors(true);

    ui->allOrdersCompany->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->allOrdersCompany->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->allOrdersCompany->verticalHeader()->hide();
    ui->allOrdersCompany->setAlternatingRowColors(true);

    ui->orderRequestsTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->orderRequestsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->orderRequestsTableView->verticalHeader()->hide();
    ui->orderRequestsTableView->setAlternatingRowColors(true);

    ui->companyOrderHistoryTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->companyOrderHistoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->companyOrderHistoryTableView->verticalHeader()->hide();
    ui->companyOrderHistoryTableView->setAlternatingRowColors(true);

    ui->customerOrderHistoryTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->customerOrderHistoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->customerOrderHistoryTableView->verticalHeader()->hide();
    ui->customerOrderHistoryTableView->setAlternatingRowColors(true);

    ui->driverOrderHistoryTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->driverOrderHistoryTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->driverOrderHistoryTableView->verticalHeader()->hide();
    ui->driverOrderHistoryTableView->setAlternatingRowColors(true);

    ui->viewInvoiceButton->setEnabled(false);
    ui->customerOrderHistoryInvoiceButton->setEnabled(false);
    ui->assignToDriverButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//############# Methods ################

void MainWindow::ClearAllInputFields(){
    foreach(QLineEdit* le, findChildren<QLineEdit*>()) {
       le->clear();
    }
}

void MainWindow::ConnectToDatabase(){
    if(!database.open()){
        //Database connection when clicked Register on Welcome Page
        database = QSqlDatabase::addDatabase("QMYSQL", "MyConnect");
        database.setHostName("127.0.0.1");
        database.setUserName("admin");
        database.setPassword("YXMOS0i3RCtj");
        database.setDatabaseName("SDI_Project");
    }

    if(database.open()){
        //Connection Success
        ui->databaseStatusRegisterLabel->setText("<font color='green'>Connected.</font>");
        ui->databaseStatusLoginLabel->setText("<font color='green'>Connected.</font>");
        ui->databaseStatusWelcomeLabel->setText("<font color='green'>Connected.</font>");
    } else{
        //Connection error
        ui->databaseStatusRegisterLabel->setText("<font color='red'>Connection failed.</font>");
        ui->databaseStatusLoginLabel->setText("<font color='red'>Connection failed.</font>");
        ui->databaseStatusWelcomeLabel->setText("<font color='red'>Connection failed.</font>");
    }
}

QString MainWindow::encryptString(QString str){
    string msg = str.toStdString();

    string encryptedText = msg;

    for (int i = 0; i < msg.size(); i++) {
        if(msg[i] == 32){ // ignore spaces
            continue;
        } else {
            encryptedText[i] += key;
        }
    }

    return QString::fromStdString(encryptedText);
}

QString MainWindow::dencryptString(QString str){
    string encpMsg = str.toStdString();

    string decryptedText = encpMsg;

    for (int i = 0; i < encpMsg.size(); i++) {
        if(encpMsg[i] == 32){ // ignore spaces
            continue;
        } else {
            decryptedText[i] = encpMsg[i] - key;
        }
    }

    return QString::fromStdString(decryptedText);
}

void MainWindow::showMyOrders(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT orderid,source,destination,totalprice,status FROM Orders WHERE ownerid=:ownerid AND status!=:status");

    qry.bindValue(":ownerid", loggedAccountID);
    qry.bindValue(":status", DELIVERED_STRING);

    qry.exec();

    myOrdersModel->setQuery(qry);
    ui->myOrdersTableView->setModel(myOrdersModel);
    ui->viewInvoiceButton->setEnabled(false);
}

void MainWindow::showAllOrders(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));


    qry.prepare("SELECT orderid,ownerid,assigneddriverid,dimension,weight,ordercondition,source,destination,comment,status,totalprice FROM Orders WHERE status = :status");

    qry.bindValue(":status", PREPARING_STRING);

    qry.exec();

    allOrdersModel->setQuery(qry);
    ui->allOrdersCompany->setModel(allOrdersModel);
    ui->assignToDriverButton->setEnabled(false);
}

void MainWindow::showCompanyOrderHistory(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT orderid,ownerid,assigneddriverid,dimension,weight,ordercondition,source,destination,comment,status,totalprice,feedback FROM Orders WHERE status = :status");

    qry.bindValue(":status", DELIVERED_STRING);

    qry.exec();

    companyOrderHistoryModel->setQuery(qry);
    ui->companyOrderHistoryTableView->setModel(companyOrderHistoryModel);
}

void MainWindow::showCustomerOrderHistory(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT orderid,assigneddriverid,dimension,weight,ordercondition,source,destination,comment,status,totalprice,feedback FROM Orders WHERE status = :status AND ownerid = :ownerid");

    qry.bindValue(":ownerid", loggedAccountID);
    qry.bindValue(":status", DELIVERED_STRING);

    qry.exec();

    customerOrderHistoryModel->setQuery(qry);
    ui->customerOrderHistoryTableView->setModel(customerOrderHistoryModel);
}

void MainWindow::showDriverOrderHistory(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT orderid,ownerid,dimension,weight,ordercondition,source,destination,comment,status,totalprice,feedback FROM Orders WHERE status = :status AND assigneddriverid = :driverid");

    qry.bindValue(":driverid", loggedAccountID);
    qry.bindValue(":status", DELIVERED_STRING);

    qry.exec();

    driverOrderHistoryModel->setQuery(qry);
    ui->driverOrderHistoryTableView->setModel(driverOrderHistoryModel);
}

void MainWindow::showDriverOrderRequests(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare("SELECT orderid,dimension,weight,ordercondition,source,destination,comment,status FROM Orders WHERE assigneddriverid = :driverid AND status = :status");

    qry.bindValue(":driverid", loggedAccountID);
    qry.bindValue(":status", CONFIRMING_STRING);

    if(qry.exec()){
        qDebug() << "Success";
    } else {
        qDebug() << "Failed";
    }

    orderRequestsModel->setQuery(qry);
    ui->orderRequestsTableView->setModel(orderRequestsModel);
}

void MainWindow::calculateShippingRate(int sizeIndex, int weightIndex){
    switch (sizeIndex) {
    case 0:
        shippingRate = 0.70;
        ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
        break;
    case 1:
        switch (weightIndex) {
        case 1:
            shippingRate = 1.06;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 2:
            shippingRate = 1.50;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 3:
            shippingRate = 2.72;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        }
        break;
    case 2:
        switch (weightIndex) {
        case 4:
            shippingRate = 3.70;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 5:
            shippingRate = 5.57;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        }
        break;
    case 3:
        switch (weightIndex) {
        case 4:
            shippingRate = 5.90;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 6:
            shippingRate = 15.85;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 7:
            shippingRate = 21.90;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        case 8:
            shippingRate = 33.40;
            ui->shippingRateLabel->setText("Shipping Rate: £" + QString::number(shippingRate, 'f', 2));
            break;
        }
        break;
    }
    calculateTotalPrice();
}

void MainWindow::calculateTotalPrice(){
    calculateCommissionFee();
    totalPrice = shippingRate + transportFee + commissionFee;

    ui->transportFeeLabel->setText("Transportation Fee: £" + QString::number(transportFee, 'f', 2));
    ui->totalPriceLabel->setText("Total: £" + QString::number(totalPrice, 'f', 2));
}

void MainWindow::calculateCommissionFee(){
    float temp = (shippingRate + transportFee) / 100;
    commissionFee = temp * 15;

    ui->commissionFeeLabel->setText("Commission Fee: £" + QString::number(commissionFee, 'f', 2));
}

void MainWindow::saveOrderToDatabase(){
    QPixmap inPixmap = ui->invoiceContainer->grab();

    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("INSERT INTO Orders (ownerid,dimension,weight,ordercondition,source,destination,comment,invoice,totalprice,status) VALUES (:ownerid,:dimension,:weight,:condition,:source,:dest,:note,:invoice,:totalprice,:status)"));

    qry.bindValue(":ownerid", loggedAccountID);
    qry.bindValue(":dimension", ui->dimensionsComboBox->currentText());
    qry.bindValue(":weight", ui->weightComboBox->currentText());
    qry.bindValue(":condition", ui->deliveryConditionComboBox->currentText());
    qry.bindValue(":source", ui->sourceComboBox->currentText());
    qry.bindValue(":dest", ui->destinationComboBox->currentText());
    qry.bindValue(":note", ui->deliveryNoteTextEdit->toPlainText());
    qry.bindValue(":invoice", inByteArray);
    qry.bindValue(":totalprice", QString::number(totalPrice, 'f', 2));
    qry.bindValue(":status", PREPARING_STRING);

    if(!qry.exec()){
        QMessageBox::warning(this, "Error 10", "Could not place the order!");
    } else {
        QMessageBox::information(this,"Success","Order successfully placed!");
        showMyOrders();
    }
}

void MainWindow::fillCurrentOrder(){
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("SELECT orderid,ownerid,source,destination,ordercondition,totalprice,comment,status FROM Orders WHERE assigneddriverid = :driverid AND status = :statusload OR status = :statusonroad"));

    qry.bindValue(":driverid", loggedAccountID);
    qry.bindValue(":statusload", LOADING_STRING);
    qry.bindValue(":statusonroad", ONROAD_STRING);

    if(qry.exec() && qry.size() > 0){
        qry.first();

        driverCurrentOrderID = qry.value(0).toInt();

        ui->driverCurrentOrderLabel->setText("Current Order ID: " + qry.value(0).toString());
        ui->driverOwnerIDLabel->setText("Owner ID: " + qry.value(1).toString());
        ui->driverSourceLabel->setText("Source: " + qry.value(2).toString());
        ui->driverDestinationLabel->setText("Destination: " + qry.value(3).toString());
        ui->driverCargoDetailsLabel->setText("Cargo Details: " + qry.value(4).toString());
        ui->driverTotalFeeLabel->setText("Total Fee: £" + qry.value(5).toString());
        ui->driverDeliveryNoteLabel->setText("Delivery Note: " + qry.value(6).toString());
        ui->driverCargoStatusLabel->setText("Cargo Status: " + qry.value(7).toString());

        if(qry.value(7).toString() == LOADING_STRING){
            ui->driverOnRoadButton->setEnabled(true);
            ui->driverDeliveredButton->setEnabled(false);
        } else if(qry.value(7).toString() == ONROAD_STRING) {
            ui->driverOnRoadButton->setEnabled(false);
            ui->driverDeliveredButton->setEnabled(true);
        }
    } else {
        ui->driverCurrentOrderLabel->setText("Current Order ID:");
        ui->driverOwnerIDLabel->setText("Owner ID:");
        ui->driverSourceLabel->setText("Source:");
        ui->driverDestinationLabel->setText("Destination:");
        ui->driverCargoDetailsLabel->setText("Cargo Details:");
        ui->driverTotalFeeLabel->setText("Total Fee:");
        ui->driverDeliveryNoteLabel->setText("Delivery Note:");
        ui->driverCargoStatusLabel->setText("Cargo Status:");

        ui->driverOnRoadButton->setEnabled(false);
        ui->driverDeliveredButton->setEnabled(false);
    }
}

//####################### DataBase - AccountInformation ######################

//-------------Registration-------------
void MainWindow::on_registerCustomerButton_clicked()
{
    // Validation

    switch (ui->accountTypeComboBox->currentIndex()) {
    case 0:
        if(ui->fullnameRegLineEdit->text() == "" || ui->usernameRegLineEdit->text() == "" || ui->passwordRegLineEdit->text() == ""
                || ui->cfmPassLineEdit->text() == "" || ui->mailRegLineEdit->text() == "" || ui->numberRegLineEdit->text() == ""
                || ui->addressRegLineEdit->text() == ""){
            QMessageBox::warning(this,"Error", "Please fill in all the required fields!");
            return;
        }
        break;
    case 1:
        if(ui->fullnameRegLineEdit->text() == "" || ui->usernameRegLineEdit->text() == "" || ui->passwordRegLineEdit->text() == ""
                || ui->cfmPassLineEdit->text() == "" || ui->mailRegLineEdit->text() == "" || ui->numberRegLineEdit->text() == ""
                || ui->addressRegLineEdit->text() == "" || ui->niNumberRegLineEdit->text() == "" || ui->drivelicenseRegLineEdit->text() == ""){
            QMessageBox::warning(this,"Error", "Please fill in all the required fields!");
            return;
        }
        break;
    case 2:
        if(ui->fullnameRegLineEdit->text() == "" || ui->usernameRegLineEdit->text() == "" || ui->passwordRegLineEdit->text() == ""
                || ui->cfmPassLineEdit->text() == "" || ui->mailRegLineEdit->text() == "" || ui->numberRegLineEdit->text() == ""
                || ui->addressRegLineEdit->text() == ""){
            QMessageBox::warning(this,"Error", "Please fill in all the required fields!");
            return;
        }
        break;
    }



    // Database
    if(ui->passwordRegLineEdit->text() == ui->cfmPassLineEdit->text()){
        if(database.open()){

            // Get data from input fields
            QString fullname = "";
            QString username = "";
            QString password = "";
            QString email = "";
            QString mobile = "";
            QString address = "";
            QString ninumber = "";
            QString drivelicense = "";
            QString accounttype = "";

            // Set account type value according to selected combobox index
            if(ui->accountTypeComboBox->currentIndex() == 0 || ui->accountTypeComboBox->currentIndex() == 1){
                // Get data from input fields
                fullname = encryptString(ui->fullnameRegLineEdit->text());
                username = encryptString(ui->usernameRegLineEdit->text());
                password = encryptString(ui->passwordRegLineEdit->text());
                email = encryptString(ui->mailRegLineEdit->text());
                mobile = encryptString(ui->numberRegLineEdit->text());
                address = encryptString(ui->addressRegLineEdit->text());
                ninumber = encryptString(ui->niNumberRegLineEdit->text());
                drivelicense = encryptString(ui->drivelicenseRegLineEdit->text());
                if(ui->accountTypeComboBox->currentIndex() == 0)
                    accounttype = "customer";
                else
                    accounttype = "driver";
            } else {
                fullname = ui->fullnameRegLineEdit->text();
                username = ui->usernameRegLineEdit->text();
                password = ui->passwordRegLineEdit->text();
                email = ui->mailRegLineEdit->text();
                mobile = ui->numberRegLineEdit->text();
                address = ui->addressRegLineEdit->text();
                ninumber = ui->niNumberRegLineEdit->text();
                drivelicense = ui->drivelicenseRegLineEdit->text();
                accounttype = "company";
            }

            // Create Query for duplicate username
            QSqlQuery qry(QSqlDatabase::database("MyConnect"));

            qry.prepare(QString("SELECT * FROM AccountInformation WHERE username = :username"));

            qry.bindValue(":username", username);

            if(!qry.exec()){ // Check if the query is successful
                QMessageBox::warning(this, "Error 01", "Could not retrieve information from the database!");
                return;
            } else if(qry.size() > 0) {
                QMessageBox::warning(this, "Error 02", "Username already exists!");
                return;
            }

            qry.prepare("INSERT INTO AccountInformation (fullname, username, password, email, mobilenumber, address, ninumber, drivinglicenseid, accounttype) "
                        "VALUES (:fullname,:username,:password,:email,:mobilenumber,:address,:ninumber,:drivinglicenseid,:accounttype)");

            qry.bindValue(":fullname", fullname);
            qry.bindValue(":username", username);
            qry.bindValue(":password", password);
            qry.bindValue(":email", email);
            qry.bindValue(":mobilenumber", mobile);
            qry.bindValue(":address", address);
            qry.bindValue(":ninumber", ninumber);
            qry.bindValue(":drivinglicenseid", drivelicense);
            qry.bindValue(":accounttype", accounttype);

            if(qry.exec()){ // Check if the query is successful
                QMessageBox::information(this,"Success","You have successfully registered!");
                ClearAllInputFields();
            } else {
                QMessageBox::warning(this,"Error 03","Could not register information to the database!");
                //QMessageBox::information(this,"Result", fullname + " " + username + " " + password + " " + email + " " + mobile + " " + address + " " + ninumber + " " + drivelicense + " " + accounttype);
            }



        } else {
            QMessageBox::warning(this,"Error 04","Connection to database is not established!");
        }
    } else {
        QMessageBox::warning(this,"Error 05", "Passwords do not match!");
    }
}



//-------------Login-------------

void MainWindow::on_loginButton_clicked()
{
    if(database.open()){

        // Retrieve data from input fields

        QString username = "";
        QString password = "";

        loggedUsername = ui->usernameLineEdit->text().toStdString();
        username = encryptString(ui->usernameLineEdit->text());

        password = encryptString(ui->passwordLineEdit->text());

        // Create Query for Encrypted Data

        QSqlQuery queryEncrypted(QSqlDatabase::database("MyConnect"));

        queryEncrypted.prepare(QString("SELECT * FROM AccountInformation WHERE username = (:username) AND password = (:password)"));

        queryEncrypted.bindValue(":username", username);
        queryEncrypted.bindValue(":password", password);

        if(!queryEncrypted.exec()){
            QMessageBox::warning(this, "Error 06", "Could not retrieve information from the database!");
        } else {

            while (queryEncrypted.next()) {

                QString usernameDB = queryEncrypted.value(2).toString();
                QString passwordDB = queryEncrypted.value(3).toString();
                QString accountTypeDB = queryEncrypted.value(9).toString();

                if(usernameDB == username && passwordDB == password){
                    ClearAllInputFields();
                    QMessageBox::information(this,"Success", "Login Success!");

                    ctID = queryEncrypted.value(0).toInt();
                    ctFullname = dencryptString(queryEncrypted.value(1).toString());
                    ctUsername = dencryptString(queryEncrypted.value(2).toString());
                    ctPassword = dencryptString(queryEncrypted.value(3).toString());
                    ctEmail = dencryptString(queryEncrypted.value(4).toString());
                    ctMobileNumber = dencryptString(queryEncrypted.value(5).toString());
                    ctAddress = dencryptString(queryEncrypted.value(6).toString());

                    loggedAccountID = queryEncrypted.value(0).toInt();

                    if(accountTypeDB == "customer"){
                        ui->mainStackedWidget->setCurrentWidget( ui->CustomerPage );
                        ui->customerUsername->setText(QString::fromStdString(loggedUsername));
                        showMyOrders();
                        showCustomerOrderHistory();
                    } else {
                        ui->mainStackedWidget->setCurrentWidget( ui->DriverPage );
                        ui->driverUsername->setText(QString::fromStdString(loggedUsername));
                        showDriverOrderRequests();
                        showDriverOrderHistory();
                        fillCurrentOrder();
                    }
                    return;
                } else {
                    QMessageBox::warning(this,"Error 07","Login information is incorrect!");
                    return;
                }
            }
        }

        username = ui->usernameLineEdit->text();

        password = ui->passwordLineEdit->text();


        // Create Query for Not Encrypted Data

        QSqlQuery queryNormal(QSqlDatabase::database("MyConnect"));

        queryNormal.prepare(QString("SELECT * FROM AccountInformation WHERE username = (:username) AND password = (:password)"));

        queryNormal.bindValue(":username", username);
        queryNormal.bindValue(":password", password);

        if(!queryNormal.exec()){
            QMessageBox::warning(this, "Error 06", "Could not retrieve information from the database!");
        } else {

            while (queryNormal.next()) {

                QString usernameDB = queryNormal.value(2).toString();
                QString passwordDB = queryNormal.value(3).toString();
                QString accountTypeDB = queryNormal.value(9).toString();

                if(usernameDB == username && passwordDB == password){
                    ClearAllInputFields();
                    QMessageBox::information(this,"Success", "Login Success!");

                    loggedAccountID = queryNormal.value(0).toInt();
                    ui->mainStackedWidget->setCurrentWidget( ui->CompanyPage );
                    ui->companyUsername->setText(QString::fromStdString(loggedUsername));
                    showAllOrders();
                    showCompanyOrderHistory();
                    return;
                } else {
                    QMessageBox::warning(this,"Error 07","Login information is incorrect!");
                    return;
                }
            }
        }
        QMessageBox::warning(this,"Error 08","Login information is incorrect!");
    } else {
        QMessageBox::warning(this,"Error 09","Connection to database is not established!");
    }
}


//####################### Navigation #######################

void MainWindow::on_welcomeRegisterButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->RegisterPage );

    ConnectToDatabase();
}

void MainWindow::on_welcomeLoginButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->LoginPage );

    ConnectToDatabase();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->welcomePage );

    ClearAllInputFields();
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->welcomePage );

    ClearAllInputFields();
}

void MainWindow::on_passwordLineEdit_textEdited(const QString &arg1)
{
    if(arg1 == ""){
        ui->loginButton->setEnabled(false);
    }else if(ui->usernameLineEdit->text() != ""){
        ui->loginButton->setEnabled(true);
    }
}

void MainWindow::on_usernameLineEdit_textEdited(const QString &arg1)
{
    if(arg1 == ""){
        ui->loginButton->setEnabled(false);
    }else if(ui->passwordLineEdit->text() != ""){
        ui->loginButton->setEnabled(true);
    }
}

void MainWindow::on_usernameLineEdit_textChanged(const QString &arg1)
{
    if(arg1 == ""){
        ui->loginButton->setEnabled(false);
    }else if(ui->passwordLineEdit->text() != ""){
        ui->loginButton->setEnabled(true);
    }
}

void MainWindow::on_accountTypeComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->drivelicenseRegLineEdit->setEnabled(false);
        ui->niNumberRegLineEdit->setEnabled(false);
        ui->cpcLineEdit->setEnabled(false);
        ui->lorryRegLineEdit->setEnabled(false);
        ClearAllInputFields();
        break;

    case 1:
        ui->drivelicenseRegLineEdit->setEnabled(true);
        ui->niNumberRegLineEdit->setEnabled(true);
        ui->cpcLineEdit->setEnabled(true);
        ui->lorryRegLineEdit->setEnabled(true);
        ClearAllInputFields();
        break;

    case 2:
        ui->drivelicenseRegLineEdit->setEnabled(false);
        ui->niNumberRegLineEdit->setEnabled(false);
        ui->cpcLineEdit->setEnabled(false);
        ui->lorryRegLineEdit->setEnabled(false);
        ClearAllInputFields();
        break;

    }
}

void MainWindow::on_pushButton_clicked()
{
    if(!database.open()){
        ConnectToDatabase();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(!database.open()){
        ConnectToDatabase();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if(!database.open()){
        ConnectToDatabase();
    }
}

// customer logout
void MainWindow::on_LogoutButton_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->welcomePage );
    loggedAccountID = -1;
    loggedUsername = "";
}

// driver logout
void MainWindow::on_LogoutButton_2_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->welcomePage );
    loggedAccountID = -1;
    loggedUsername = "";
}

// company logout
void MainWindow::on_LogoutButton_3_clicked()
{
    ui->mainStackedWidget->setCurrentWidget( ui->welcomePage );
    loggedAccountID = -1;
    loggedUsername = "";
}

void MainWindow::on_customerProfileButton_clicked()
{
    customerprofile cprofile(this);
    cprofile.setAccountInformation(key, ctID);
    cprofile.setModal(true);
    cprofile.exec();

}

void MainWindow::on_pushButton_6_clicked()
{
    showMyOrders();
}

void MainWindow::on_pushButton_7_clicked()
{
    showAllOrders();
}

void MainWindow::on_dimensionsComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->weightComboBox->setCurrentIndex(0);
        ui->weightComboBox->setItemData(0,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(1,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(2,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(3,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(4,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(5,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(6,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(7,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(8,0,Qt::UserRole - 1);
        break;
    case 1:
        ui->weightComboBox->setCurrentIndex(1);
        ui->weightComboBox->setItemData(0,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(1,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(2,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(3,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(4,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(5,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(6,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(7,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(8,0,Qt::UserRole - 1);
        break;
    case 2:
        ui->weightComboBox->setCurrentIndex(4);
        ui->weightComboBox->setItemData(0,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(1,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(2,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(3,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(4,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(5,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(6,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(7,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(8,0,Qt::UserRole - 1);
        break;
    case 3:
        ui->weightComboBox->setCurrentIndex(4);
        ui->weightComboBox->setItemData(0,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(1,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(2,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(3,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(4,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(5,0,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(6,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(7,33,Qt::UserRole - 1);
        ui->weightComboBox->setItemData(8,33,Qt::UserRole - 1);
        break;
    }

    calculateShippingRate(ui->dimensionsComboBox->currentIndex(), ui->weightComboBox->currentIndex());
}

void MainWindow::on_weightComboBox_currentIndexChanged(int index)
{
    calculateShippingRate(ui->dimensionsComboBox->currentIndex(), index);
}

void MainWindow::on_driverProfileButton_clicked()
{
    driverprofile dprofile(this);
    dprofile.setAccountInformation(key, ctID);
    dprofile.setModal(true);
    dprofile.exec();
}

void MainWindow::on_sourceComboBox_currentIndexChanged(int index)
{
    for (int i = 0; i < ui->destinationComboBox->count(); i++) {
        ui->destinationComboBox->setItemData(i,33,Qt::UserRole - 1);
    }
    ui->destinationComboBox->setItemData(index,0,Qt::UserRole - 1);

    transportFee = 10.00 + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(20.00-10.00)));

    calculateTotalPrice();
}

void MainWindow::on_destinationComboBox_currentIndexChanged(int index)
{
    for (int i = 0; i < ui->sourceComboBox->count(); i++) {
        ui->sourceComboBox->setItemData(i,33,Qt::UserRole - 1);
    }
    ui->sourceComboBox->setItemData(index,0,Qt::UserRole - 1);

    transportFee = 10.00 + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(20.00-10.00)));

    calculateTotalPrice();
}

void MainWindow::on_deliveryNoteTextEdit_textChanged()
{
    if(ui->deliveryNoteTextEdit->toPlainText().length() > maxDeliveryNoteLength)
    {
    int diff = ui->deliveryNoteTextEdit->toPlainText().length() - maxDeliveryNoteLength;
    QString newStr = ui->deliveryNoteTextEdit->toPlainText();
    newStr.chop(diff);
    ui->deliveryNoteTextEdit->setText(newStr);
    QTextCursor cursor(ui->deliveryNoteTextEdit->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    ui->deliveryNoteTextEdit->setTextCursor(cursor);
    }

    ui->deliveryNoteLabel->setText("Delivery Note (" + QString::number(ui->deliveryNoteTextEdit->toPlainText().length()) + "/" + QString::number(maxDeliveryNoteLength) + "):");
}

void MainWindow::on_placeOrderButton_clicked()
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Order Confirmation",tr("Do you confirm this order?\n"),
                                                               QMessageBox::Cancel | QMessageBox::Yes, QMessageBox::Yes);
    if(resBtn == QMessageBox::Yes){
        saveOrderToDatabase();
    }
}

void MainWindow::on_myOrdersTableView_clicked(const QModelIndex &index)
{
    QItemSelectionModel* selection = ui->myOrdersTableView->selectionModel();
    QList<QModelIndex> idxes = selection->selectedIndexes();

    selectedOrderID = ui->myOrdersTableView->model()->data(idxes[0]).toInt();

    ui->viewInvoiceButton->setEnabled(true);
}

void MainWindow::on_viewInvoiceButton_clicked()
{
    invoiceViewer ivprofile(this);
    ivprofile.setOrderID(selectedOrderID);
    ivprofile.setModal(true);
    ivprofile.exec();
}

void MainWindow::on_allOrdersCompany_clicked(const QModelIndex &index)
{
    QItemSelectionModel* selection = ui->allOrdersCompany->selectionModel();
    QList<QModelIndex> idxes = selection->selectedIndexes();

    companySelectedOrderID = ui->allOrdersCompany->model()->data(idxes[0]).toInt();

    ui->assignToDriverButton->setEnabled(true);
}

void MainWindow::on_assignToDriverButton_clicked()
{
    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("SELECT accountid FROM AccountInformation WHERE accounttype = (:actype) AND driverbusy = (:driverbusy)"));

    qry.bindValue(":actype", "driver");
    qry.bindValue(":driverbusy", 0);

    if(!qry.exec()){
        QMessageBox::warning(this, "Error 12", "Could not retrieve information from the database!");
    } else {

        if(qry.size() > 0){
            qry.first();
            int assignedDriverID = qry.value(0).toInt();

            QSqlQuery driverQry(QSqlDatabase::database("MyConnect"));
            driverQry.prepare(QString("UPDATE AccountInformation SET driverbusy = :driverbusy WHERE accountid = :driveraccid"));
            driverQry.bindValue(":driverbusy", 1);
            driverQry.bindValue(":driveraccid", assignedDriverID);

            if(!driverQry.exec()){
                QMessageBox::warning(this, "Error 14", "Could not update information on database!");
            }

            QSqlQuery assignQry(QSqlDatabase::database("MyConnect"));

            assignQry.prepare(QString("UPDATE Orders SET status = :status, assigneddriverid = :assigned WHERE orderid = :orderid"));

            assignQry.bindValue(":status", CONFIRMING_STRING);
            assignQry.bindValue(":orderid", companySelectedOrderID);
            assignQry.bindValue(":assigned", assignedDriverID);

            if(!assignQry.exec()){
                QMessageBox::warning(this, "Error 13", "Could not update information on database!");
            } else {
                QMessageBox::information(this, "Success", "Successfully assigned the order to a driver!");
                showAllOrders();
            }
        } else {
            QMessageBox::warning(this, "Error 11", "There are no available drivers at this moment!");
        }
    }
}

void MainWindow::on_orderRequestsTableView_clicked(const QModelIndex &index)
{
    QItemSelectionModel* selection = ui->orderRequestsTableView->selectionModel();
    QList<QModelIndex> idxes = selection->selectedIndexes();

    int selectedOrderID = ui->orderRequestsTableView->model()->data(idxes[0]).toInt();

    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Order Request",tr("Do you want to accept this order?\n"),
                                                               QMessageBox::No | QMessageBox::Yes, QMessageBox::Yes);
    if(resBtn == QMessageBox::Yes){
        QSqlQuery qry(QSqlDatabase::database("MyConnect"));

        qry.prepare(QString("UPDATE Orders SET status = :status, assigneddriverid = :assigned WHERE orderid = :orderid"));

        qry.bindValue(":status", LOADING_STRING);
        qry.bindValue(":orderid", selectedOrderID);
        qry.bindValue(":assigned", loggedAccountID);

        if(!qry.exec()){
            QMessageBox::warning(this, "Error 14", "Could not update information on database!");
        } else {
            QMessageBox::information(this, "Order Request", "Accepted the order!");
            showDriverOrderRequests();
            fillCurrentOrder();
            qDebug() << "Accepted" << selectedOrderID << "Order";
        }
    } else {

        QSqlQuery qry(QSqlDatabase::database("MyConnect"));

        qry.prepare(QString("UPDATE Orders SET status = :status, assigneddriverid = :assigned WHERE orderid = :orderid"));

        qry.bindValue(":status", PREPARING_STRING);
        qry.bindValue(":assigned", 0);
        qry.bindValue(":orderid", selectedOrderID);

        if(!qry.exec()){
            QMessageBox::warning(this, "Error 15", "Could not update information on database!");
        } else {
            QSqlQuery qry(QSqlDatabase::database("MyConnect"));

            qry.prepare(QString("UPDATE AccountInformation SET driverbusy = :busy WHERE accountid = :accountid"));

            qry.bindValue(":busy", 0);
            qry.bindValue(":accountid", loggedAccountID);

            if(!qry.exec()){
                QMessageBox::warning(this, "Error 15", "Could not update information on database!");
            } else {
                QMessageBox::information(this, "Order Request", "Rejected the order!");
                showDriverOrderRequests();
                qDebug() << "Rejected" << selectedOrderID << "Order";
            }
        }
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    showDriverOrderRequests();
}

void MainWindow::on_driverOnRoadButton_clicked()
{
    ui->driverOnRoadButton->setEnabled(false);
    ui->driverDeliveredButton->setEnabled(true);

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("UPDATE Orders SET status = :status WHERE orderid = :orderid"));

    qry.bindValue(":status", ONROAD_STRING);
    qry.bindValue(":orderid", driverCurrentOrderID);

    if(!qry.exec()){
        qDebug() << "Failed to change status";
    } else {
        fillCurrentOrder();
    }
}

void MainWindow::on_driverDeliveredButton_clicked()
{
    ui->driverOnRoadButton->setEnabled(false);
    ui->driverDeliveredButton->setEnabled(false);

    QSqlQuery qry(QSqlDatabase::database("MyConnect"));

    qry.prepare(QString("UPDATE Orders SET status = :status WHERE orderid = :orderid"));

    qry.bindValue(":status", DELIVERED_STRING);
    qry.bindValue(":orderid", driverCurrentOrderID);

    if(!qry.exec()){
        qDebug() << "Failed to change status";
    } else {
        QSqlQuery qry(QSqlDatabase::database("MyConnect"));

        qry.prepare(QString("UPDATE AccountInformation SET driverbusy = :busy WHERE accountid = :accountid"));

        qry.bindValue(":busy", 0);
        qry.bindValue(":accountid", loggedAccountID);

        if(!qry.exec()){
            qDebug() << "Failed to change account busy status.";
        } else {
            feedback feedback(this);
            feedback.setCurrentOrderID(driverCurrentOrderID);
            feedback.setModal(true);
            feedback.exec();
        }

        fillCurrentOrder();
    }
}

void MainWindow::on_companyOrderHistoryRefreshButton_clicked()
{
    showCompanyOrderHistory();
}

void MainWindow::on_customerOrderHistoryRefreshButton_clicked()
{
    showCustomerOrderHistory();
    ui->customerOrderHistoryInvoiceButton->setEnabled(false);
}

void MainWindow::on_customerOrderHistoryInvoiceButton_clicked()
{
    invoiceViewer ivprofile(this);
    ivprofile.setOrderID(customerHistorySelectedOrderID);
    ivprofile.setModal(true);
    ivprofile.exec();
}

void MainWindow::on_customerOrderHistoryTableView_clicked(const QModelIndex &index)
{
    ui->customerOrderHistoryInvoiceButton->setEnabled(true);

    QItemSelectionModel* selection = ui->customerOrderHistoryTableView->selectionModel();
    QList<QModelIndex> idxes = selection->selectedIndexes();

    customerHistorySelectedOrderID = ui->customerOrderHistoryTableView->model()->data(idxes[0]).toInt();
}

void MainWindow::on_driverOrderHistoryRefreshButton_clicked()
{
    showDriverOrderHistory();
}
