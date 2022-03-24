#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
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

    void on_accountTypeComboBox_currentIndexChanged(int index);

    void on_loginButton_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase database;
};
#endif // MAINWINDOW_H
