/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef UI_SELECT_TIMETAG_DEVICE_H
#define UI_SELECT_TIMETAG_DEVICE_H

#include <QDialog>
#include <QListWidgetItem>

#include "../support/timetag_device.h"

namespace Ui {
class ui_select_timetag_device;
}

class ui_select_timetag_device : public QDialog
{
    Q_OBJECT

public:
    struct selected_device
    {
        bool any_item_selected;
        timetag_device device;
    };

private:
    std::vector<timetag_device> devices;

    void add_devices();

    selected_device sel_device;

public:
    explicit ui_select_timetag_device(QWidget *parent, const std::vector<timetag_device>& devices);
    ~ui_select_timetag_device();

    selected_device DeviceSelected() const;
private slots:
    void on_lw_items_itemChanged(QListWidgetItem *item);

    void on_lw_items_itemClicked(QListWidgetItem *item);

private:
    Ui::ui_select_timetag_device *ui;
};

#endif // UI_SELECT_TIMETAG_DEVICE_H
