//----------------------------------------------------------------------------
//  PROJECT :   ESP01-TEST
//  PURPOSE :   Sending AT commands to the ESP01 serial device.
//----------------------------------------------------------------------------

#include <QApplication>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QObject>
#include <QStringList>
#include <QDebug>

#include "utilities.h"
#include "mainwindow.h"
#include "esp01.h"
#include "commobject.h"

//static QStringList commandList = {
//    "AT",
//    "AT+CIFSR",
//    "AT+CWJAP?",
//    "AT+GMR",
//    "AT+CIPSTATUS",
//    "AT+CWLAP",
//    "ATS"
//};


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#if 0
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
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

    commObj.close();

    return a.exec();
}
#endif
