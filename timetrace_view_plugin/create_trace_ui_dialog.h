/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef CREATE_TRACE_UI_DIALOG_H
#define CREATE_TRACE_UI_DIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include <phast_gui/support/typedefs.h>

namespace Ui {
class create_trace_ui_dialog;
}

class create_trace_ui_dialog : public QDialog
{
    Q_OBJECT

public:
    struct init_vals
    {
        std::vector<chan_id> channels;
        bool sum_values;
        uint64_t bin_width;
    };

    create_trace_ui_dialog(const create_trace_ui_dialog&) = delete;
    create_trace_ui_dialog& operator=(const create_trace_ui_dialog&) = delete;
    explicit create_trace_ui_dialog(QWidget *parent,
                                    const std::vector<chan_id>& channels,
                                    uint64_t time_unit_hundreths_of_ps);
    ~create_trace_ui_dialog();

    init_vals GetValues() const;

private slots:
    void on_list_chans_itemClicked(QListWidgetItem *item);

    void on_cnt_num_bins_valueChanged(double value);

    void on_chk_sum_chans_toggled(bool checked);

private:
    std::vector<chan_id> channels;
    uint64_t time_unit;

    Ui::create_trace_ui_dialog *ui;

    init_vals p;

    void setup_chanlist();
};

#endif // CREATE_TRACE_UI_DIALOG_H
