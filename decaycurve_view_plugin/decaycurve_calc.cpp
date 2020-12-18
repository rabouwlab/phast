/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "decaycurve_calc.h"
#include <phast_gui/support/dataview.h>
#include <phast_gui/interfaces/isettingscontainer.h>

#include <libtimetag/algos.h>
#include <iostream>

decaycurve_calc::decaycurve_calc(const std::vector<chan_id>& chans_to_sum,
                                 uint64_t bin_width,
                                 opaque_ptr our_ref,
                                 double bin_low,
                                 double bin_high,
                                 bool display_only_initial_data,
                                 double time_unit) :
    Base_ui_calc(chans_to_sum, bin_width, our_ref, display_only_initial_data, time_unit),
    bin_low(bin_low),
    bin_high(bin_high)
{
}

void decaycurve_calc::ClearData()
{
    this->xs.clear();
    this->ys.clear();

    this->data_displayed = false;
}

chan_data_map decaycurve_calc::get_times_to_save(DetectionUpdate event) const
{
    chan_data_map ret;

    for (auto pair : event.updates) {
        chan_det_update update = pair.second;
        ret[pair.first] = update.macro_end; // Everything can be trashed.
    }

    return ret;
}

DataView::data_update decaycurve_calc::arrange_update(DetectionUpdate event, bool rebin_only)
{
    DataView::data_update update;

    if (this->xs.size() == 0) {
        int64_t len = linspace_len(this->bin_low, this->bin_high, 1, 1, 1);
        this->xs.resize(len, 0);
        linspace(this->bin_low,
                 this->bin_high,
                 1,
                 1,
                 1,
                 this->xs.data(),
                 this->xs.size());

        this->ys.resize(this->xs.size() - 1, 0);
    }

    if (!rebin_only) {

        for (uint64_t i = 0; i < this->chans_to_sum.size(); i++) {
            uint64_t chan = this->chans_to_sum.at(i);

            if (event.updates.count(chan) == 0)
                continue;

            chan_det_update upd = event.updates.at(chan);

            if (upd.n_new_micro == 0)
                continue;

            // TODO: reimplement decay offset time
            const int64_t* data_ptr = nullptr;
            int64_t data_len = 0;

            data_ptr = &*upd.new_micro_start;
            data_len = upd.n_new_micro;

            /*
            if (ci.micro_display_delaytime == 0) {
                data_ptr = &*upd.new_micro_start;
                data_len = upd.n_new_micro;
            } else {
                // Apply the delay time
                temp_data.resize(static_cast<uint64_t>(upd.n_new_micro));

                for (int64_t i = 0; i < upd.n_new_micro; i++) {
                    temp_data[i] = *(upd.new_micro_start + i) + ci.micro_display_delaytime;
                }

                data_ptr = temp_data.data();
                data_len = temp_data.size();
            }
            */

            int success = bindata_interp_seq(this->xs.data(),
                                           this->xs.size(),
                                         data_ptr,
                                            data_len,
                                           this->ys.data(),
                                           this->ys.size());

            if (success != 0) {
                std::cerr << "Error binning decay data :-(" << std::endl;
            }

        }
    }


    std::vector<int64_t> use_xs(rebin_bin_edges_len(this->xs.size(), this->bin_width), 0);
    std::vector<int64_t> use_ys(rebin_len(this->ys.size(), this->bin_width), 0);

    if (this->bin_width == 1) {
        use_xs = this->xs;
        use_ys = this->ys;
    } else {
        rebin(this->ys.data(),
              this->ys.size(),
              this->bin_width,
              use_ys.data(),
              use_ys.size());
        rebin_bin_edges(this->xs.data(),
                        this->xs.size(),
                        this->bin_width,
                        use_xs.data(),
                        use_xs.size());
    }

    for (uint64_t i = 0; i < use_xs.size(); i++) {
        update.xs.push_back((double)use_xs[i]*this->time_unit*1e9);
    }

    for (uint64_t i = 0; i < use_ys.size(); i++) {
        update.ys.push_back((double)use_ys[i]);
    }

    update.chan_num = -1;
    update.sender = this->our_ref;

    return update;
}
