/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef DETECTIONUPDATE_H
#define DETECTIONUPDATE_H

#include <map>

#include "../support/typedefs.h"

class chan_det_update
{
public:
    data_type micro_start;
    data_type micro_end;
    data_type new_micro_start;

    int64_t n_micro;
    int64_t n_new_micro;
    int64_t n_old_micro;

    data_type macro_start;
    data_type macro_end;
    data_type new_macro_start;

    int64_t n_macro;
    int64_t n_new_macro;
    int64_t n_old_macro;


    chan_det_update(data_type micro_start,
                    data_type micro_end,
                    data_type new_micro_start,
                    data_type macro_start,
                    data_type macro_end,
                    data_type new_macro_start) :
        micro_start(micro_start),
        micro_end(micro_end),
        new_micro_start(new_micro_start),
        n_micro(std::distance(micro_start, micro_end)),
        n_new_micro(std::distance(new_micro_start, micro_end)),
        n_old_micro(std::distance(micro_start, new_micro_start)),
        macro_start(macro_start),
        macro_end(macro_end),
        new_macro_start(new_macro_start),
        n_macro(std::distance(macro_start, macro_end)),
        n_new_macro(std::distance(new_macro_start, macro_end)),
        n_old_macro(std::distance(macro_start, new_macro_start))
    {
    }

    chan_det_update& operator=(const chan_det_update& o)
    {
        this->micro_start = o.micro_start;
        this->micro_end = o.micro_end;
        this->new_macro_start = o.new_macro_start;

        this->n_macro = o.n_macro;
        this->new_micro_start = o.new_micro_start;
        this->n_micro = o.n_micro;

        this->n_new_micro = o.n_new_micro;
        this->n_old_micro = o.n_old_micro;
        this->macro_start = o.macro_start;

        this->macro_end = o.macro_end;
        this->n_new_macro = o.n_new_macro;
        this->n_old_macro = o.n_old_macro;

        return *this;
    }

    chan_det_update(const chan_det_update& o) :
        micro_start(o.micro_start),
        micro_end(o.micro_end),
        new_micro_start(o.new_micro_start),
        n_micro(o.n_micro),
        n_new_micro(o.n_new_micro),
        n_old_micro(o.n_old_micro),
        macro_start(o.macro_start),
        macro_end(o.macro_end),
        new_macro_start(o.new_macro_start),
        n_macro(o.n_macro),
        n_new_macro(o.n_new_macro),
        n_old_macro(o.n_old_macro)
    {
    }

    chan_det_update() :
        micro_start(),
        micro_end(),
        new_micro_start(),
        n_micro(0),
        n_new_micro(0),
        n_old_micro(0),
        macro_start(),
        macro_end(),
        new_macro_start(),
        n_macro(0),
        n_new_macro(0),
        n_old_macro(0)
    {

    }
};

struct DetectionUpdate
{
public:
    const std::map<chan_id, chan_det_update>& updates;

    explicit DetectionUpdate(const std::map<chan_id, chan_det_update>& updates) :
        updates(updates)
    {
    }
};

#endif // DETECTIONUPDATE_H
