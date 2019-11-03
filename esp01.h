#ifndef SERIALPORTCONTROLLER_H
#define SERIALPORTCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

using SerialPortList = QList<QSerialPortInfo>;

class ESP01 : public QObject
{
    Q_OBJECT
public:
    explicit ESP01(QObject *parent = nullptr);
    ~ESP01();

    static SerialPortList enumerateSerialPorts(bool dump=false);

    bool    open(QString portName = QString());
    void    close();

    bool    sendCommand(QString cmd, QStringList & result);

signals:

public slots:

protected:
    QString         serPortName;
    QSerialPort     serPort;

    bool            configurePort();
};


#endif // SERIALPORTCONTROLLER_H
