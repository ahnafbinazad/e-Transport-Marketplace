#ifndef CUSTOMERPROFILE_H
#define CUSTOMERPROFILE_H

#include <QDialog>

namespace Ui {
class customerprofile;
}

class customerprofile : public QDialog
{
    Q_OBJECT

public slots:
    void setAccountInformation(int, int);

public:
    explicit customerprofile(QWidget *parent = nullptr);
    ~customerprofile();

private slots:
    void on_cProfileSaveChanges_clicked();

private:
    Ui::customerprofile *ui;
};

#endif // CUSTOMERPROFILE_H
