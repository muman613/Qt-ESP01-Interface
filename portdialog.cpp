#include <QSerialPort>
#include <QSerialPortInfo>

#include "portdialog.h"
#include "ui_portdialog.h"

portDialog::portDialog(QWidget *parent, QString current) :
    QDialog(parent),
    ui(new Ui::portDialog)
{
    ui->setupUi(this);

    auto serialPorts = QSerialPortInfo::availablePorts();

    for (auto port : serialPorts) {
        ui->portCombo->addItem(port.portName());
    }

    if (!current.isEmpty()) {
        ui->portCombo->setCurrentText(current);
    }
}

portDialog::~portDialog()
{
    delete ui;
}

QString portDialog::getPort()
{
    return ui->portCombo->currentText();
}
