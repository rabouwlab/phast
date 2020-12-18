/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "customscaledraw.h"

#include <math.h>

CustomScaleDraw::CustomScaleDraw(bool do_not_show_biggest_val) :
    QwtScaleDraw(),
    biggest_val(0),
    do_not_show_biggest_val(do_not_show_biggest_val)
{
}

void CustomScaleDraw::set_biggest_val(double value)
{
    this->biggest_val = value;
}

QwtText CustomScaleDraw::label(double value) const
{
    if (do_not_show_biggest_val && value >= this->biggest_val) {

        CustomScaleDraw* csd = const_cast<CustomScaleDraw*>(this);

        csd->set_biggest_val(value);
        return QString();
    }

    QwtText ret;

    /*
    if (value >= 1e6 || value <= -1e6) {
        ret = QString::number(value*1e-6, 'f', 1) + "M";
    } else if (value >= 1e3 || value <= -1e3) {
        ret = QString::number(value*1e-3, 'f', 1) + "k";
    } else {
        ret = QString::number(value, 'f', 1);
    }
    */

    // Scientific notation
    // e.g. 1e4 instead of 10 000
    // and  1e-2 instead of 0.01
    if (value == 0.0) {
        ret = QString("0");
    } else if (value >= 1e3 || value <= 1e-3) {
        // Get power

        double log = (value < 0) ? log10(-value) : log10(value);
        double log_ = trunc(log);
        double leftover = value/(pow(10, log_));

        ret = QString::number(leftover, 'f', 2) + "e" + QString::number(log_);
    } else {
        ret = QString::number(value, 'f', 2);
    }
    return ret;
}
