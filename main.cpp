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

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setOrganizationName("softwaremagic");
    a.setApplicationName("esp01-test");

    MainWindow w;
    w.show();

    return a.exec();
}
