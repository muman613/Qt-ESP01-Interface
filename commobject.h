#ifndef COMMOBJECT_H
#define COMMOBJECT_H

#include <QObject>
#include <QTimer>
#include <QMutex>
#include <QAtomicInteger>
#include "esp01.h"


enum commState {
    COMM_UNINIT,
    COMM_IDLE,
    COMM_COMMAND_PENDING,
};

class CommObject : public QObject
{
    Q_OBJECT

public:
    explicit    CommObject(QObject *parent = nullptr);
    virtual     ~CommObject();

    bool        isOpen() const;
    bool        isPending() const;

    bool        open(QString portName = QString());
    void        close();

    int         atCommand(QString command, QStringList * response = nullptr, int timeout = 5000);

    QString     getSerialPort() const;
    void        getVersion(QString & atVersion,
                           QString & sdkVersion,
                           QString & compileTime);
    int         getMode();
    void        setMode();

signals:

public slots:
    void onCommandResponse(bool status, QStringList response);
    void onTimeout();

protected:
    ESP01 *     esp01 = nullptr;

    QTimer      timer1;

    enum commState state = COMM_UNINIT;

    QMutex                  commMutex;
    QString                 serialPort;
    QString                 defaultPort;
    bool                    bCmdResult        = false;
    QStringList             responseArray;
    QAtomicInteger<bool>    bResponseReceived = false;
    QAtomicInteger<bool>    bReqTimedOut      = false;

};

#endif // COMMOBJECT_H
