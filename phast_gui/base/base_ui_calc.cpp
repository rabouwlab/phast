/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_ui_calc.h"
#include "../support/dataview.h"

Base_ui_calc::Base_ui_calc(const std::vector<chan_id> &chans_to_sum,
                               uint64_t bin_width,
                               opaque_ptr our_ref,
                           bool display_only_initial_data,
                           double time_unit) :
    chans_to_sum(chans_to_sum),
    our_ref(our_ref),
    bin_width(bin_width),
    data_displayed(false),
    m(),
    time_unit(time_unit),
    display_only_initial_data(display_only_initial_data)
{
}

void Base_ui_calc::clear_data()
{
    std::lock_guard<std::mutex> l(this->m);

    this->ClearData();
}

const chan_data_map Base_ui_calc::NewEvent(ITempDataStorage *sender, DetectionUpdate event)
{
    chan_data_map ret = this->get_times_to_save(event);

    std::lock_guard<std::mutex> l(this->m);

    this->recalc(event);

    this->data_displayed = true;

    return ret;
}

void Base_ui_calc::on_new_bin_width(uint64_t width)
{
    this->bin_width = width;
}

uint64_t Base_ui_calc::get_bin_width() const
{
    return this->bin_width;
}

void Base_ui_calc::recalc(DetectionUpdate event)
{
    if (this->chans_to_sum.size() == 0) {
        return;
    }


    DataView::data_update update = this->arrange_update(event,
                                                         (this->display_only_initial_data && this->data_displayed) ? true : false);
    update.sender = this->our_ref;

    if (this->chans_to_sum.size() > 1) {
        update.chan_num = -1;
    } else if (this->chans_to_sum.size() == 1) {
        update.chan_num = this->chans_to_sum[0];
    }

    emit new_data(update);
}
