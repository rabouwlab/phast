/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TIMETRACE_CALC_H
#define TIMETRACE_CALC_H

#include <QObject>

#include <phast_gui/base/base_ui_calc.h>

class timetrace_calc : public Base_ui_calc
{
private:
    std::vector<int64_t> xs;
    std::vector<int64_t> ys;

    int64_t first_bin_lower;
    bool first_time;

protected:
    virtual chan_data_map get_times_to_save(DetectionUpdate event) const;

    virtual DataView::data_update arrange_update(DetectionUpdate event, bool rebin_only=false);

public:
    timetrace_calc(const std::vector<chan_id>& chans_to_sum,
                   uint64_t bin_width,
                   opaque_ptr our_ref, bool display_only_initial_data, double time_unit);

    virtual void ClearData();
};

#endif // TIMETRACE_CALC_H
