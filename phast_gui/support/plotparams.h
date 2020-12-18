/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef PLOTPARAMS_H
#define PLOTPARAMS_H

struct plot_params
{
public:
    double xmin;
    double xmax;
    bool x_log_scale;
    bool xmin_autoscale;
    bool xmax_autoscale;

    bool keep_x_const_size;
    double x_const_size;

    double ymin;
    double ymax;
    bool y_log_scale;
    bool ymin_autoscale;
    bool ymax_autoscale;
    bool keep_y_const_size;
    double y_const_size;
};

#endif // PLOTPARAMS_H
