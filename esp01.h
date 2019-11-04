#ifndef SERIALPORTCONTROLLER_H
#define SERIALPORTCONTROLLER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

using SerialPortList = QList<QSerialPortInfo>;

enum state {
    DISCONNECTED,
    IDLE,
    WAIT_RESULT,
};

class ESP01 : public QObject
{
    Q_OBJECT
public:
    explicit ESP01(QObject *parent = nullptr);
    ~ESP01();

    static SerialPortList enumerateSerialPorts(bool dump=false);

    bool    open(QString portName = QString());
    void    close();
    bool    isOpen() const;

    void    sendCommand(QString cmd);

signals:
    void    commandResponse(bool status, QStringList response);

public slots:
    void onReadyRead();

protected:
    QString         serPortName;
    QSerialPort     serPort;
    bool            cmdSent = false;
    enum state      curState = DISCONNECTED;
    bool            configurePort();

    QString         replyLineBuffer;
    QStringList     response;
};


#endif // SERIALPORTCONTROLLER_H
