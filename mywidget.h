#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QtCore>
#include <string>
#ifndef MYWIDGET_H
#define MYWIDGET_H

QT_BEGIN_NAMESPACE
using namespace std;
QT_END_NAMESPACE

class MyWidget : public QWidget
{
public:
    MyWidget(){
        timer = new QTimer(this);
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showMyOrders()));
        timer->start(1000);
    }
private:
    QTimer *timer;
};

#endif // MYWIDGET_H
