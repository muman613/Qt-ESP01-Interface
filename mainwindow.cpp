#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "mainwindow.h"
#include "portdialog.h"
#include "ui_mainwindow.h"
#include "commobject.h"

using QCommandPair       = QPair<QString, QString>;
using QCommandPairVector = QVector<QCommandPair>;

static QCommandPairVector commandVec = {
    QCommandPair("Test AT startup",             "AT"),
    QCommandPair("Check version info",          "AT+GMR"),
    QCommandPair("Get local IP address",        "AT+CIFSR"),
    QCommandPair("Query connected AP",          "AT+CWJAP?"),
    QCommandPair("Get connection status",       "AT+CIPSTATUS"),
    QCommandPair("List available APs",          "AT+CWLAP"),
    QCommandPair("Set Station Mode",            "AT+CWMODE=1"),
    QCommandPair("Set SoftAP Mode",             "AT+CWMODE=2"),
    QCommandPair("Set SoftAP & Station Mode",   "AT+CWMODE=3"),
    QCommandPair("Ping sfgate.com",             "AT+PING=\"sfgate.com\""),
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pComObject = new CommObject(this);
    pComObject->open();

    for (auto & cmd : commandVec) {
        ui->command->addItem(cmd.first);
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
    QString command = commandVec[ui->command->currentIndex()].second;

    if (pComObject->atCommand(command, &resp) == 0) {
        for (auto & line : resp) {
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

    for (auto & port : serialPorts) {
        QString line;

        line = QString("%1 %2 %3").arg(port.portName(), -15).arg(port.description(), -30).arg(port.manufacturer(), -20);

        ui->textBox->append(line);
    }
}
