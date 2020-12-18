/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ZOOMER_H
#define ZOOMER_H

#include <qwt_plot_zoomer.h>

class Zoomer: public QwtPlotZoomer
{
public:
    Zoomer( int xAxis, int yAxis, QWidget *canvas ):
        QwtPlotZoomer( xAxis, yAxis, canvas )
    {
        // Show the coordinates when the selection is active
        this->setTrackerMode( QwtPicker::ActiveOnly );

        // No 'rubber band' (whatever that may be)
        this->setRubberBand( QwtPicker::NoRubberBand );

        // zoom out completely
        this->setMousePattern( QwtEventPattern::KeyHome,
            Qt::RightButton, Qt::ControlModifier );

        // zoom out one step
        this->setMousePattern( QwtEventPattern::KeyUndo,
            Qt::RightButton );
    }
};

#endif // ZOOMER_H
