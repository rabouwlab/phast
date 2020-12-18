#include "offline_online_dialog.h"
#include "ui_offline_online_dialog.h"

offline_online_dialog::offline_online_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::offline_online_dialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

    this->state = NONE;
}

offline_online_dialog::~offline_online_dialog()
{
    delete ui;
}

void offline_online_dialog::on_btn_online_clicked()
{
    this->state = ONLINE;

    this->close();
}

void offline_online_dialog::on_btn_offline_clicked()
{
    this->state = OFFLINE;

    this->close();
}
