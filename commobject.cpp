#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QThread>
#include "commobject.h"
#include "utilities.h"

CommObject::CommObject(QObject *parent)
    : QObject(parent),
      commMutex(QMutex::Recursive)
{
#ifdef ENABLE_TIMER
    connect(&timer1, &QTimer::timeout, this, &CommObject::onTimeout);
    timer1.start(5000);
#endif
}

bool CommObject::isOpen() const
{
    bool result = false;

    if (esp01) {
        result = esp01->isOpen();
    }

    return result;
}

bool CommObject::open(QString portName)
{
    if (esp01 != nullptr) {
        delete esp01;
        esp01 = nullptr;
    }

    esp01 = new ESP01(this);
    bool result =  esp01->open(portName);
    if (result) {
        connect(esp01, &ESP01::commandResponse, this, &CommObject::onCommandResponse);
    }

//    atCommand("AT+GMR");

    return result;
}

void CommObject::close()
{
    if (esp01) {
        esp01->close();
        delete esp01;
        esp01 = nullptr;
    }
}

int CommObject::atCommand(QString command, QStringList * response)
{
//    qDebug() << Q_FUNC_INFO << command;

    int result = 0;

    if (response) {
        response->clear();
    }

    if (esp01) {
        esp01->sendCommand(command);

        // Wait for response to be returned...
        while (bResponseReceived != true) {
            QCoreApplication::processEvents();
            QThread::usleep(100);
        }
        bResponseReceived = false;

        if (bCmdResult) {
//            displayResults(responseArray);
            if (response) {
                *response = responseArray;
            }
        } else {
            result = -1;
        }
    } else {
        result = -10;
    }

    return result;
}

void CommObject::onCommandResponse(bool status, QStringList response)
{
    QMutexLocker lock(&commMutex);

    bCmdResult = status;
    responseArray = response;
    bResponseReceived = true;
}

void CommObject::onTimeout()
{
#if 0
    qDebug() << "Scanning Wifi networks...";

    atCommand("AT+CWLAP");
#endif
}

