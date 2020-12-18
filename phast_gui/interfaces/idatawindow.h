/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef IDATAWINDOW_H
#define IDATAWINDOW_H

#include <QObject>

class IDataPlot;
class IBinningClass;
class ITempDataStorage;

class IDataWindow : public QObject
{
    Q_OBJECT

public:
    virtual ~IDataWindow() {}

public slots:
    virtual void clear_data() = 0;
    virtual void on_new_tempdatastorage(ITempDataStorage*) = 0;
    virtual void show_window() = 0;

signals:
   void got_focus(IDataPlot* plot);
   void got_binner_focus(IBinningClass* binner);
   void plot_deleted(IDataPlot* plot);
   void plot_binner_deleted(IBinningClass* binner);
};

#endif // IDATAWINDOW_H
