#include "selectserialportdialog.h"
#include "ui_selectserialportdialog.h"

selectSerialPortDialog::selectSerialPortDialog(QWidget *parent, const QList<QString> &boardNames,int *selectedPort) :
    QDialog(parent),
    ui(new Ui::selectSerialPortDialog)
{
    ui->setupUi(this);
    for(int i=0;i<boardNames.count();i++)
        ui->comboBoxSerialPortSelection->addItem(boardNames[i]);
    this->selectedPort = selectedPort;
    connect(ui->comboBoxSerialPortSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(selectedPortChanged(int)));
}

selectSerialPortDialog::~selectSerialPortDialog()
{
    delete ui;
}

void selectSerialPortDialog::selectedPortChanged(int selectedPort)
{
    *(this->selectedPort) =selectedPort;
    return;
}
