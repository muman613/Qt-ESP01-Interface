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

#include "utilities.h"
#include "esp01.h"


static QStringList commandList = {
    "AT",
    "AT+CIFSR",
    "AT+CWJAP?",
    "AT+GMR",
    "AT+CIPSTATUS",
    "AT+CWLAP",
    "ATS"
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ESP01       esp01;

    if (esp01.open()) {
        for (QString command : commandList) {
            QStringList result;

            if (esp01.sendCommand(command, result)) {
                displayResults(result);
            } else {
                printf("ERROR: Command '%s' failed\n",
                       command.toLocal8Bit().data());
            }
        }
    }
    esp01.close();

    qDebug() << "DONE";

    return a.exec();
}
