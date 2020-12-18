/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "timetrace_calc.h"

#include <libtimetag/algos.h>
#include <iostream>

timetrace_calc::timetrace_calc(const std::vector<chan_id> &chans_to_sum,
                               uint64_t bin_width,
                               opaque_ptr our_ref,
                               bool display_only_initial_data,
                               double time_unit) :
    Base_ui_calc(chans_to_sum, bin_width, our_ref, display_only_initial_data, time_unit),
    first_bin_lower(0),
    first_time(true)
{
}

void timetrace_calc::ClearData()
{
    this->xs.clear();
    this->ys.clear();

    this->data_displayed = false;

    this->first_time = true;
}

chan_data_map timetrace_calc::get_times_to_save(DetectionUpdate event) const
{
    chan_data_map ret;

    for (auto pair : event.updates) {
        chan_det_update upd = pair.second;

        chan_id id = pair.first;
        ret[id] = upd.macro_end; // Everything can be trashed.
    }

    return ret;
}

DataView::data_update timetrace_calc::arrange_update(DetectionUpdate event, bool rebin_only)
{
    DataView::data_update ret;
    // TODO: this displays nothing for really low count rates?

    if (!rebin_only) {
        // TODO: choose the last bin such, that it makes sense
        // with the GUI update times. Otherwise, we see nothing when we have a count rate of zero!
        timestamp t_max = 0;
        timestamp t_min = INT64_MAX;

        bool any_new_macro = false;

        for (chan_id ID : this->chans_to_sum) {
            if (event.updates.count(ID) == 0)
                continue;

            chan_det_update upd = event.updates.at(ID);

            if (upd.n_new_macro == 0)
                continue;

            any_new_macro = true;

            timestamp macro = *(upd.macro_end - 1);
            if (macro > t_max) {
                t_max = macro;
            }

            timestamp macro2 = *upd.macro_start;

            if (macro2 < t_min) {
                t_min = macro2;
            }
        }

        // bin upper should be chosen such, that the last bin contains the last photon.
        double bin_width = (1e-3)/this->time_unit; // Fundamental bin width: 1 ms
        uint64_t bin_width_u = (uint64_t)bin_width;

        int64_t bin_lower = 0;

        if (first_time) {
            if (any_new_macro) {
                bin_lower = t_min;
                this->first_bin_lower = t_min;
                first_time = false;
            }
        }

        bin_lower = this->first_bin_lower;

        int64_t bin_upper = ceil(t_max / bin_width) * bin_width_u;

        // TODO: generate bins if necessary...
        int64_t n_bins = linspace_len(bin_lower, bin_upper, bin_width_u, 1, 1);

        if (n_bins <= 0) {
            return ret;
        }

        this->xs.resize(n_bins, 0);

        linspace(bin_lower, bin_upper, bin_width_u, 1, 1, this->xs.data(), n_bins);

        this->ys.resize(n_bins - 1, 0);

        for (uint64_t i = 0; i < this->chans_to_sum.size(); i++) {
            uint64_t chan = this->chans_to_sum.at(i);

            if (event.updates.count(chan) == 0)
                continue;

            chan_det_update upd = event.updates.at(chan);

            if (upd.n_new_macro == 0)
                continue;

            int success = bindata_interp_seq(this->xs.data(),
                                               this->xs.size(),
                                               &*upd.new_macro_start,
                                               upd.n_new_macro,
                                               this->ys.data(),
                                               this->ys.size());

            if (success != 0) {
                std::cerr << "Error binning intensity data :-(" << std::endl;
            }
        }
    }

    if (this->xs.size() == 0 ||
            this->ys.size() == 0) {
        return ret;
    }    

    std::vector<int64_t> rebinned_xs(rebin_bin_edges_len(this->xs.size(), this->bin_width));
    std::vector<int64_t> rebinned_ys(rebin_len(this->ys.size(), this->bin_width));

    if (this->bin_width > 1) {
        rebin(this->ys.data(),
              this->ys.size(),
              this->bin_width,
              rebinned_ys.data(),
              rebinned_ys.size());

        rebin_bin_edges(this->xs.data(),
                        this->xs.size(),
                        this->bin_width,
                        rebinned_xs.data(),
                        rebinned_xs.size());
    } else {
        rebinned_xs = this->xs;
        rebinned_ys = this->ys;
    }

    std::vector<double> xs2;
    std::vector<double> ys2;

    if (rebinned_xs.size() < 2 ||
            rebinned_ys.size() < 2) {
        ret.xs = xs2;
        ret.ys = ys2;

        return ret;
    }

    double first_x = (double)rebinned_xs[0]*this->time_unit;

    for (uint64_t i = 0; i < rebinned_xs.size() - 1; i++) {
        xs2.push_back((double)rebinned_xs[i]*this->time_unit - first_x);
    }

    for (uint64_t i = 0; i < rebinned_ys.size() - 1; i++) {
        ys2.push_back((double)rebinned_ys[i]);
    }

    ret.xs = xs2;
    ret.ys = ys2;

    return ret;
}
