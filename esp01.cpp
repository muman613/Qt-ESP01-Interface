#include <QDebug>
#include "esp01.h"

ESP01::ESP01(QObject *parent) : QObject(parent)
{
//    qDebug() << Q_FUNC_INFO;
}

ESP01::~ESP01()
{
//    qDebug() << Q_FUNC_INFO;

    close();
}

SerialPortList ESP01::enumerateSerialPorts(bool dump)
{
    auto serialPorts = QSerialPortInfo::availablePorts();

    if (dump) {
        for (auto serialPort : serialPorts) {
            printf("%20s %40s %20s\n",
                   serialPort.portName().toLocal8Bit().data(),
                   serialPort.description().toLocal8Bit().data(),
                   serialPort.systemLocation().toLocal8Bit().data());
        }
    }

    return serialPorts;
}

bool ESP01::open(QString portName)
{
    if (portName.isEmpty()) {
        auto portList = enumerateSerialPorts();
        if (!portList.isEmpty()) {
            portName = portList.at(0).systemLocation();
        } else {
            qDebug() << "ERROR: No serial ports found";
            return false;
        }
    }
    serPortName = portName;

    serPort.setPortName(serPortName);
    serPort.setBaudRate(QSerialPort::Baud115200);
    serPort.setDataBits(QSerialPort::Data8);
    serPort.setParity(QSerialPort::NoParity);
    serPort.setStopBits(QSerialPort::OneStop);
    serPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!serPort.open(QIODevice::ReadWrite)) {
        qDebug() << "Unable to open serial port" << serPortName;
        return false;
    }
    return true;
}

void ESP01::close()
{
    if (serPort.isOpen()) {
        serPort.close();
    }
}

/**
 * @brief sendCommand - send command to ESP01 Wifi device
 * @param port - serial port to communicate with.
 * @param cmd - String containing AT command to send.
 * @param result - Array of strings returned from command
 * @return - true on OK result, false on ERROR result.
 */
bool ESP01::sendCommand(QString cmd, QStringList & result) {
    bool ret = false;

    result.clear();

    // If the input command doesn't end with LF/CR add it to the command...
    if (!cmd.endsWith("\r\n")) {
        cmd += "\r\n";
    }
    serPort.write(cmd.toLocal8Bit());

    bool bDone = false;

    serPort.waitForReadyRead();

    QString full_line;

    while (!bDone) {

        QString line = QString(serPort.readLine());
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
        if (!serPort.bytesAvailable()) {
            serPort.waitForReadyRead();
        }
    }

    return ret;
}
