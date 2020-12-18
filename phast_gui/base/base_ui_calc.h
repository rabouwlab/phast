/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BASE_UI_CALC_H
#define BASE_UI_CALC_H

#include "../interfaces/ilistener.h"
#include "../support/detectionupdate.h"
#include "../support/dataview.h"
#include "../support/typedefs.h"
#include "../interfaces/ibinningclass.h"
#include "../support/global.h"

#include <QObject>
#include <mutex>

class TT_EXPORT Base_ui_calc : public QObject, public IListener, public IBinningClass
{
    Q_OBJECT

protected:
    std::vector<chan_id> chans_to_sum;
    opaque_ptr our_ref;
    uint64_t bin_width;
    bool data_displayed;
    std::mutex m;
    double time_unit;

private:
    bool display_only_initial_data;

    void recalc(DetectionUpdate event);

protected:
    virtual chan_data_map get_times_to_save(DetectionUpdate event) const = 0;

    virtual DataView::data_update arrange_update(DetectionUpdate event, bool rebin_only) = 0;

    virtual void ClearData() = 0;

public:
    Base_ui_calc(const std::vector<chan_id>& chans_to_sum,
                   uint64_t bin_width,
                   opaque_ptr our_ref,
                 bool display_only_initial_data,
                 double time_unit);

   virtual uint64_t get_bin_width() const override;

   // Should return time from which deletion is not allowed yet.
   //  (all times smaller than this time may be removed)
   virtual const chan_data_map NewEvent(ITempDataStorage* sender, DetectionUpdate event) override;


    void clear_data();

public slots:
     virtual void on_new_bin_width(uint64_t width) override;

signals:
     void new_data(DataView::data_update data);
};

#endif // BASE_UI_CALC_H
