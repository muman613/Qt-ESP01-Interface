#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commobject.h"

static QStringList commandList = {
    "AT",
    "AT+CIFSR",
    "AT+CWJAP?",
    "AT+GMR",
    "AT+CIPSTATUS",
    "AT+CWLAP",
    "AT+CWMODE=1",
    "AT+CWMODE=2",
    "AT+CWMODE=3",
    "AT+CWMODE?",
    "ATS"
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    pComObject = new CommObject(this);

    pComObject->open();

    if (pComObject->isOpen()) {
        qDebug() << "CommObject is open!";

        for (auto cmd : commandList) {
            ui->command->addItem(cmd);
        }

        pComObject->GetVersion(versionInfo[0], versionInfo[1], versionInfo[2]);

        QString sMesg = QString("AT Version = %1").arg(versionInfo[0]);

        ui->textBox->append(sMesg);

    } else {
        ui->command->setEnabled(false);
        ui->execute->setEnabled(false);
        ui->textBox->setEnabled(false);

        ui->textBox->append("Unable to open comm device");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_execute_clicked()
{
    QStringList resp;

    qDebug() << Q_FUNC_INFO;

    QString command = ui->command->currentText();

    if (pComObject->atCommand(command, &resp) == 0) {
        for (auto line : resp) {
            ui->textBox->append(line);
        }
    } else {
        QString status = QString("Error sending command '%1'").arg(command);
        ui->textBox->append(status);
    }
}
