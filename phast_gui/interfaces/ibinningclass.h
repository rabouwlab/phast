/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IBINNINGCLASS_H
#define IBINNINGCLASS_H

#include "stdint.h"

class IBinningClass {

public:
    virtual ~IBinningClass() {}

    virtual void on_new_bin_width(uint64_t width) = 0;
    virtual uint64_t get_bin_width() const = 0;
};

#endif // IBINNINGCLASS_H
