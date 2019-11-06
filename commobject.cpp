#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QMutexLocker>
#include <QThread>
#include <QSettings>
#include "commobject.h"
#include "utilities.h"

CommObject::CommObject(QObject *parent)
    : QObject(parent),
      commMutex(QMutex::Recursive)
{
    // Get default comm port
    QSettings settings(this);

    if (settings.status() == QSettings::NoError) {
        settings.beginGroup("wifi");
        defaultPort = settings.value("port", QVariant::fromValue(QString(""))).toString();
        settings.endGroup();
    }

    connect(&timer1, &QTimer::timeout, this, &CommObject::onTimeout);
}

CommObject::~CommObject()
{
    // Save port setting
    QSettings settings(this);
    if (settings.status() == QSettings::NoError) {
        settings.beginGroup("wifi");
        settings.setValue("port", QVariant::fromValue(serialPort));
        settings.endGroup();
    }
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
        close();
    }

    esp01 = new ESP01(this);

    // If user did not pass portName, use the deaultPort
    if (portName.isEmpty()) {
        portName = defaultPort;
    }

    bool result =  esp01->open(portName);
    if (result) {
        serialPort = portName;
        connect(esp01, &ESP01::commandResponse, this, &CommObject::onCommandResponse);
    }

    return result;
}

void CommObject::close()
{
    if (esp01) {
        disconnect(esp01, &ESP01::commandResponse, this, &CommObject::onCommandResponse);

        esp01->close();
        delete esp01;
        esp01 = nullptr;
    }
}

int CommObject::atCommand(QString command, QStringList * response, int timeout)
{
    QMutexLocker lock(&commMutex);

//    qDebug() << Q_FUNC_INFO << command;

    int result = 0;

    if (response) {
        response->clear();
    }

    if (esp01) {
        esp01->sendCommand(command);

        qDebug() << "Starting timeout timer";
        timer1.setSingleShot(true);
        timer1.start(timeout);

        // Wait for response to be returned...
        while ((bResponseReceived != true) && (bReqTimedOut == false)) {
            QCoreApplication::processEvents();
            QThread::usleep(100);
        }

        if (bResponseReceived) {
            bResponseReceived = false;

            if (bCmdResult) {
                if (response) {
                    *response = responseArray;
                }
            } else {
                result = -1;
            }
        } else if (bReqTimedOut) {
            qDebug() << "Request timed out";
            bReqTimedOut = false;
            result = -2;
        }

        qDebug() << "Stopping timeout timer";
        timer1.stop();
        bReqTimedOut = bResponseReceived = false;
    } else {
        result = -10;
    }

    return result;
}

QString CommObject::getSerialPort() const {
    return serialPort;
}

void CommObject::getVersion(QString &atVersion, QString &sdkVersion, QString &compileTime)
{
    if (isOpen()) {
        QStringList response;

        if (atCommand("AT+GMR", &response, 100) == 0) {
            qDebug() << response;
            atVersion = response[1].section(':', 1);
            sdkVersion = response[2].section(':', 1);
            compileTime = response[3].section(':', 1);
        }
    }
}

int CommObject::getMode()
{
    int mode = 0;

    if (isOpen()) {
        QStringList response;

        if (atCommand("AT+CWMODE?", &response, 100) == 0) {
            qDebug() << response;
            mode = response[1].section(':', 1).toInt();
        }
    }

    return mode;
}

void CommObject::setMode()
{

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
    qDebug() << Q_FUNC_INFO;
    bReqTimedOut = true;
}

