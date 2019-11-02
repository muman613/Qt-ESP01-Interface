#include <QDebug>
#include "serialportcontroller.h"

SerialPortController::SerialPortController(QObject *parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO;
}

SerialPortController::~SerialPortController()
{
    qDebug() << Q_FUNC_INFO;
}
