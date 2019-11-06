#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CommObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_execute_clicked();

    void on_actionExit_triggered();

    void on_actionSelect_serial_device_triggered();

    void on_actionEnumerate_serial_devices_triggered();

private:
    void updateConnection();

    Ui::MainWindow *ui;

    CommObject * pComObject = nullptr;

    QString     versionInfo[3];

};
#endif // MAINWINDOW_H
