//----------------------------------------------------------------------------
//  PROJECT :   ESP01-TEST
//  PURPOSE :   Sending AT commands to the ESP01 serial device.
//----------------------------------------------------------------------------

#include <QCoreApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QStringList>
#include <QDebug>

#include "serialportcontroller.h"

using SerialPortList = QList<QSerialPortInfo>;

SerialPortList enumerateSerialPorts(bool dump=false) {
    auto serialPorts = QSerialPortInfo::availablePorts();

    if (dump) {
        for (auto serialPort : serialPorts) {
            qDebug() << serialPort.portName() << serialPort.description() << serialPort.systemLocation();
        }
    }

    return serialPorts;
}

QString getFirstSerialport() {
    auto serialPorts = enumerateSerialPorts();

    if (serialPorts.empty()) {
        return QString();
    }

    return serialPorts[0].systemLocation();
}

/**
 * @brief sendCommand - send command to ESP01 Wifi device
 * @param port - serial port to communicate with.
 * @param cmd - String containing AT command to send.
 * @param result - Array of strings returned from command
 * @return - true on OK result, false on ERROR result.
 */
bool sendCommand(QSerialPort & port, QString cmd, QStringList & result) {
    bool ret = false;

    result.clear();

    // If the input command doesn't end with LF/CR add it to the command...
    if (!cmd.endsWith("\r\n")) {
        cmd += "\r\n";
    }
    port.write(cmd.toLocal8Bit());

    bool bDone = false;

    port.waitForReadyRead();

    QString full_line;

    while (!bDone) {

        QString line = QString(port.readLine());
        line.remove("\r");

        if (line == "OK\n") {
            bDone = true;
            ret = true;
            continue;
        } else if (line == "ERROR\n") {
            bDone = true;
            ret = false;
            continue;
        } else {
            if (line.endsWith("\n")) {
                line.remove("\n");
                full_line += line;
                if (!full_line.isEmpty()) {
                    result.push_back(full_line);
                }
                full_line.clear();
            } else {
                full_line += line;
            }
        }
        // If we have not received the final result wait for more data to arive...
        if (!port.bytesAvailable()) {
            port.waitForReadyRead();
        }
    }

    return ret;
}

void drawLine() {
    for (int i = 0 ; i < 80 ; i++)
        printf("*");
    printf("\n");

}
void displayResults(QStringList & results) {
    drawLine();
    for (auto line : results) {
        printf(">> %s\n", line.toLocal8Bit().data());
    }
    drawLine();
}

static QStringList commandList = {
    "AT",
    "AT+CIFSR",
    "AT+GMR",
    "AT+CIPSTATUS",
    "AT+CWLAP",
    "ATS"
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    enumerateSerialPorts(true);

    QString portPath = getFirstSerialport();

    qDebug() << "Using 1st port found " << portPath;

    QSerialPort port(portPath);

    port.setBaudRate(QSerialPort::Baud115200);
    port.setDataBits(QSerialPort::Data8);
    port.setParity(QSerialPort::NoParity);
    port.setStopBits(QSerialPort::OneStop);
    port.setFlowControl(QSerialPort::NoFlowControl);

    QStringList result;

    if (port.open(QIODevice::ReadWrite)) {
        qDebug() << "Port is opened for reading & writing";

        for (QString command : commandList) {
            if (sendCommand(port, command, result)) {
                displayResults(result);
            } else {
                printf("ERROR: Command '%s' failed\n",
                       command.toLocal8Bit().data());
            }
        }

        port.close();

    }

    qDebug() << "DONE";

    return a.exec();
}
