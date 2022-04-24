#ifndef DRIVERPROFILE_H
#define DRIVERPROFILE_H

#include <QDialog>

namespace Ui {
class driverprofile;
}

class driverprofile : public QDialog
{
    Q_OBJECT

public slots:
    void setAccountInformation(int, int);

public:
    explicit driverprofile(QWidget *parent = nullptr);
    ~driverprofile();

private slots:
    void on_dProfileSaveChanges_clicked();

private:
    Ui::driverprofile *ui;
};

#endif // DRIVERPROFILE_H
