#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QDialog>

namespace Ui {
class feedback;
}

class feedback : public QDialog
{
    Q_OBJECT

public slots:
    void setCurrentOrderID(int);

public:
    explicit feedback(QWidget *parent = nullptr);
    ~feedback();

private slots:
    void on_driverSubmitButton_clicked();

private:
    Ui::feedback *ui;
};

#endif // FEEDBACK_H
