#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->usernameLineEdit->setPlaceholderText("Enter Your Username");
    ui->passwordLineEdit->setPlaceholderText("Enter Your Username");
    ui->databaseStatusRegisterLabel->setText("<font color='black'>Waiting...</font>");
    ui->databaseStatusLoginLabel->setText("<font color='black'>Waiting...</font>");
    ui->databaseStatusWelcomeLabel->setText("<font color='black'>Waiting...</font>");
    ConnectToDatabase();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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


//####################### DataBase ######################AccountInformation#

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
            QString fullname = ui->fullnameRegLineEdit->text();
            QString username = ui->usernameRegLineEdit->text();
            QString password = ui->passwordRegLineEdit->text();
            QString email = ui->mailRegLineEdit->text();
            QString mobile = ui->numberRegLineEdit->text();
            QString address = ui->addressRegLineEdit->text();
            QString ninumber = ui->niNumberRegLineEdit->text();
            QString drivelicense = ui->drivelicenseRegLineEdit->text();
            QString accounttype = "";

            // Set account type value according to selected combobox index
            switch (ui->accountTypeComboBox->currentIndex()) {
            case 0:
                accounttype = "customer";
                break;

            case 1:
                accounttype = "driver";
                break;

            case 2:
                accounttype = "company";
                break;
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

            qry.prepare("INSERT INTO AccountInformation (fullname, username, password, email, mobilenumber, address, ninumber, drivinglicenseid, accounttype, photograph) "
                        "VALUES (:fullname,:username,:password,:email,:mobilenumber,:address,:ninumber,:drivinglicenseid,:accounttype,:photograph)");

            qry.bindValue(":fullname", fullname);
            qry.bindValue(":username", username);
            qry.bindValue(":password", password);
            qry.bindValue(":email", email);
            qry.bindValue(":mobilenumber", mobile);
            qry.bindValue(":address", address);
            qry.bindValue(":ninumber", ninumber);
            qry.bindValue(":drivinglicenseid", drivelicense);
            qry.bindValue(":accounttype", accounttype);
            qry.bindValue(":photograph", "");

            if(qry.exec()){ // Check if the query is successful
                QMessageBox::information(this,"Success","You have successfully registered!");
                ClearAllInputFields();
            } else {
                QMessageBox::warning(this,"Error 03","Could not register information to the database!");
                QMessageBox::information(this,"Result", fullname + " " + username + " " + password + " " + email + " " + mobile + " " + address + " " + ninumber + " " + drivelicense + " " + accounttype);
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

        QString username = ui->usernameLineEdit->text();
        QString password = ui->passwordLineEdit->text();

        // Create Query

        QSqlQuery query(QSqlDatabase::database("MyConnect"));

        query.prepare(QString("SELECT * FROM AccountInformation WHERE username = :username AND password = :password"));

        query.bindValue(":username", username);
        query.bindValue(":password", password);

        if(!query.exec()){
            QMessageBox::warning(this, "Error 06", "Could not retrieve information from the database!");
        } else {

            while (query.next()) {

                QString usernameDB = query.value(2).toString();
                QString passwordDB = query.value(3).toString();
                QString accountTypeDB = query.value(9).toString();

                if(usernameDB == username && passwordDB == password){
                    ClearAllInputFields();
                    QMessageBox::information(this,"Success", "Login Success!");

                    if(accountTypeDB == "customer"){
                        ui->mainStackedWidget->setCurrentWidget( ui->CustomerPage );
                    } else if(accountTypeDB == "driver"){
                        ui->mainStackedWidget->setCurrentWidget( ui->DriverPage );
                    } else {
                        ui->mainStackedWidget->setCurrentWidget( ui->CompanyPage );
                    }
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
        ClearAllInputFields();
        break;

    case 1:
        ui->drivelicenseRegLineEdit->setEnabled(true);
        ui->niNumberRegLineEdit->setEnabled(true);
        ClearAllInputFields();
        break;

    case 2:
        ui->drivelicenseRegLineEdit->setEnabled(false);
        ui->niNumberRegLineEdit->setEnabled(false);
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
