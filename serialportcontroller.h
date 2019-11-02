#ifndef SERIALPORTCONTROLLER_H
#define SERIALPORTCONTROLLER_H

#include <QObject>

class SerialPortController : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortController(QObject *parent = nullptr);
    ~SerialPortController();

signals:

public slots:
};


#endif // SERIALPORTCONTROLLER_H
