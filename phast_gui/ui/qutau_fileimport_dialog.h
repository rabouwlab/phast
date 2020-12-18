/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef QUTAU_FILEIMPORT_DIALOG_H
#define QUTAU_FILEIMPORT_DIALOG_H

#include <QDialog>

namespace Ui {
class qutau_fileimport_dialog;
}

class qutau_fileimport_dialog : public QDialog
{
    Q_OBJECT

public:
    struct dialog_info
    {
    public:
        QString filename;
        bool pulses_chan_defined;
        uint64_t pulses_channel;
        bool trash_useless_pulses;
    };

    qutau_fileimport_dialog(const qutau_fileimport_dialog&) = delete;
    qutau_fileimport_dialog& operator=(const qutau_fileimport_dialog&) = delete;
    explicit qutau_fileimport_dialog(QWidget *parent = nullptr);
    ~qutau_fileimport_dialog();

    dialog_info info;

private slots:
    void on_chk_have_pulses_stateChanged(int arg1);

    void on_cnt_pulses_chan_ID_valueChanged(int arg1);

    void on_chk_neglect_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::qutau_fileimport_dialog *ui;

};

#endif // QUTAU_FILEIMPORT_DIALOG_H
