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
#include "commobject.h"

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
    CommObject commObj(&a);

    if (commObj.open()) {
        qDebug() << "Commobject is open";

        for (auto cmd : commandList) {
            QStringList cmdResponse;

            if (commObj.atCommand(cmd, &cmdResponse) == 0) {
                displayResults(cmdResponse);
            } else {
                qDebug() << "ERROR on command " << cmd;
            }
        }
    }

    return a.exec();
}
