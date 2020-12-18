#ifndef OFFLINE_ONLINE_DIALOG_H
#define OFFLINE_ONLINE_DIALOG_H

#include <QDialog>

#include "offline_online.h"

namespace Ui {
class offline_online_dialog;
}

class offline_online_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit offline_online_dialog(QWidget *parent = 0);
    ~offline_online_dialog();

    LineState state;

private slots:
    void on_btn_online_clicked();

    void on_btn_offline_clicked();

private:
    Ui::offline_online_dialog *ui;
};

#endif // OFFLINE_ONLINE_DIALOG_H
