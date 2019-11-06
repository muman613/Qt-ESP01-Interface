#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"
#include "portdialog.h"
#include "ui_mainwindow.h"
#include "commobject.h"

static QStringList commandList = {
    "AT",
    "AT+CIFSR",
    "AT+CWJAP?",
    "AT+GMR",
    "AT+CIPSTATUS",
    "AT+CWLAP",
    "AT+CWMODE=1",
    "AT+CWMODE=2",
    "AT+CWMODE=3",
    "AT+CWMODE?",
    "AT+PING=\"sfgate.com\"",
    "ATS"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pComObject = new CommObject(this);
    pComObject->open();

    for (auto cmd : commandList) {
        ui->command->addItem(cmd);
    }

    updateConnection();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_execute_clicked()
{
    QStringList resp;

    QString command = ui->command->currentText();

    if (pComObject->atCommand(command, &resp) == 0) {
        for (auto line : resp) {
            ui->textBox->append(line);
        }
    } else {
        QString status = QString("Error sending command '%1'").arg(command);
        ui->textBox->append(status);
    }
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionSelect_serial_device_triggered()
{
    portDialog  portDlg(this, pComObject->getSerialPort());

    if (portDlg.exec() == QDialog::Accepted) {
        QString serPort =  portDlg.getPort();

        qDebug() << "User selected port " << serPort;

        pComObject->close();
        pComObject->open(serPort);
        updateConnection();
    }
}

void MainWindow::enableControls(bool bEnable) {
    ui->command->setEnabled(bEnable);
    ui->execute->setEnabled(bEnable);
    ui->textBox->setEnabled(bEnable);
}
void MainWindow::updateConnection() {
//    qDebug() << Q_FUNC_INFO;

    if (pComObject->isOpen()) {
        qDebug() << "CommObject is open!";

        // Send AT command to test if we are connected to a 8266 device...
        if (pComObject->atCommand("AT", nullptr, 100) != 0) {
            ui->textBox->append("Doesn't appear to be a 8266 device!");
            enableControls(false);
            return;
        }

        pComObject->getVersion(versionInfo[0], versionInfo[1], versionInfo[2]);

        QString sMesg = QString("AT Version = %1").arg(versionInfo[0]);

        ui->textBox->append(sMesg);

        int mode = pComObject->getMode();
        qDebug() << "mode = " << mode;

        enableControls(true);
    } else {
        enableControls(false);
        ui->textBox->append("Unable to open comm device");
    }

}

void MainWindow::on_actionEnumerate_serial_devices_triggered()
{
    auto serialPorts = QSerialPortInfo::availablePorts();

    ui->textBox->append(QString("%1 %2 %3").arg("Port Name", -15).arg("Description", -30).arg("Manufacturer", -20));

    ui->textBox->append(QString(80, '-'));

    for (auto port : serialPorts) {
        QString line;

        line = QString("%1 %2 %3").arg(port.portName(), -15).arg(port.description(), -30).arg(port.manufacturer(), -20);

        ui->textBox->append(line);
    }
}
