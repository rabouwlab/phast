/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef QUTAU_FILEREADER_H
#define QUTAU_FILEREADER_H

#include <stdint.h>

#include <map>
#include <vector>

#include <QObject>

#include "chaninfo.h" // TODO

#include "../support/photon_event.h"
#include "../support/typedefs.h"
#include "../support/raw_photon_data.h"

class quTAU_filereader : public QObject
{
    Q_OBJECT

signals:
    void signalProgress(int);
    void finished();

public:
    /*
        read_quTau function

        Parameters:
            filepath                The filepath to the input file.
            time_unit_in_seconds    The time unit to use for the data, in seconds.
                                    E.g. 1e-9 for a clock of 1 ns.
        Output:
            A timetagged_file struct, containing the processed file.

        Note:
            This function was created based on the documentation from:

            [1]: "quTAU quPSI Manual v4.0", qutools, 2016, pp 25
    */
    bool read_quTau(const char* filepath,
                    bool trash_useless_pulses,
                    std::map<uint64_t,chaninfo>* chaninfos,
                    chan_dat_holder *datas,
                    int64_t pulses_chan = -1,
                    double time_unit_in_seconds=81e-12);

    bool read_quTau(const char *filepath,
                  bool trash_useless_pulses,
                  std::map<uint64_t, chaninfo> *chaninfos,
                  raw_photon_data* data,
                  int64_t pulses_chan=-1,
                  double time_unit_in_seconds=81e-12);
};

#endif // QUTAU_FILEREADER_H
