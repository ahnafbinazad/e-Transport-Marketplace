#ifndef INVOICEVIEWER_H
#define INVOICEVIEWER_H

#include <QDialog>

namespace Ui {
class invoiceViewer;
}

class invoiceViewer : public QDialog
{
    Q_OBJECT

public slots:
    void setOrderID(int);
    void viewInvoice();

public:
    explicit invoiceViewer(QWidget *parent = nullptr);
    ~invoiceViewer();

private:
    Ui::invoiceViewer *ui;
};

#endif // INVOICEVIEWER_H
