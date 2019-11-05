#ifndef PORTDIALOG_H
#define PORTDIALOG_H

#include <QDialog>

namespace Ui {
class portDialog;
}

class portDialog : public QDialog
{
    Q_OBJECT

public:
    explicit portDialog(QWidget *parent, QString current);
    ~portDialog();

    QString getPort();

private:
    Ui::portDialog *ui;
};

#endif // PORTDIALOG_H
