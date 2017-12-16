#ifndef SELECTSERIALPORTDIALOG_H
#define SELECTSERIALPORTDIALOG_H

#include <QDialog>

namespace Ui {
class selectSerialPortDialog;
}

class selectSerialPortDialog : public QDialog
{
    Q_OBJECT

public:
    explicit selectSerialPortDialog(QWidget *parent = 0,const QList<QString> &boardNames = QList<QString>(),int *selectedPort = nullptr);
    ~selectSerialPortDialog();

private:
    Ui::selectSerialPortDialog *ui;
    int *selectedPort;
private slots:
    void selectedPortChanged(int selectedPort);

};

#endif // SELECTSERIALPORTDIALOG_H
