/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAPLOT_H
#define IDATAPLOT_H

#include <qobject.h>

#include "../support/plotparams.h"

class IDataPlot : public QObject
{
    Q_OBJECT

public:
    virtual ~IDataPlot() {}

    virtual plot_params get_plot_params() const = 0;

public slots:
    virtual void set_plot_params(plot_params value) = 0;
    virtual void do_plot() = 0;
};

#endif // IDATAPLOT_H
