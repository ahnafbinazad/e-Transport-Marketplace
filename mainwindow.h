#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QtCore>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
using namespace std;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QSqlDatabase database;
    int key = 3;
    ~MainWindow();
private slots:

    QString encryptString(QString);

    QString dencryptString(QString);

    void calculateShippingRate(int, int);

    void showMyOrders();

    void showAllOrders();

    void showCompanyOrderHistory();

    void showCustomerOrderHistory();

    void showDriverOrderHistory();

    void showDriverOrderRequests();

    void calculateTotalPrice();

    void calculateCommissionFee();

    void saveOrderToDatabase();

    void on_welcomeRegisterButton_clicked();

    void on_welcomeLoginButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_passwordLineEdit_textEdited(const QString &arg1);

    void on_usernameLineEdit_textEdited(const QString &arg1);

    void on_registerCustomerButton_clicked();

    void on_usernameLineEdit_textChanged(const QString &arg1);

    void ClearAllInputFields();

    void ConnectToDatabase();

    void fillCurrentOrder();

    void on_accountTypeComboBox_currentIndexChanged(int index);

    void on_loginButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_LogoutButton_clicked();

    void on_LogoutButton_2_clicked();

    void on_LogoutButton_3_clicked();

    void on_customerProfileButton_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_dimensionsComboBox_currentIndexChanged(int index);

    void on_weightComboBox_currentIndexChanged(int index);

    void on_driverProfileButton_clicked();

    void on_sourceComboBox_currentIndexChanged(int index);

    void on_destinationComboBox_currentIndexChanged(int index);

    void on_deliveryNoteTextEdit_textChanged();

    void on_placeOrderButton_clicked();

    void on_myOrdersTableView_clicked(const QModelIndex &index);

    void on_viewInvoiceButton_clicked();

    void on_allOrdersCompany_clicked(const QModelIndex &index);

    void on_assignToDriverButton_clicked();

    void on_orderRequestsTableView_clicked(const QModelIndex &index);

    void on_pushButton_8_clicked();

    void on_driverOnRoadButton_clicked();

    void on_driverDeliveredButton_clicked();

    void on_companyOrderHistoryRefreshButton_clicked();

    void on_customerOrderHistoryRefreshButton_clicked();

    void on_customerOrderHistoryInvoiceButton_clicked();

    void on_customerOrderHistoryTableView_clicked(const QModelIndex &index);

    void on_driverOrderHistoryRefreshButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
