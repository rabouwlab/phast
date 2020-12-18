/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef CUSTOMSCALEDRAW_H
#define CUSTOMSCALEDRAW_H

#include <qwt/qwt_scale_draw.h>

class CustomScaleDraw : public QwtScaleDraw
{
private:
    double biggest_val;
    bool do_not_show_biggest_val;

    void set_biggest_val(double value);

public:
    explicit CustomScaleDraw(bool do_not_show_biggest_val=false);

    virtual QwtText label(double value) const;
};

#endif // CUSTOMSCALEDRAW_H
