/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef BASE_VIEW_PLOT_H
#define BASE_VIEW_PLOT_H

#include <QDialog>
#include <QListWidgetItem>

#include <qwt/qwt_plot_curve.h>
#include "../support/typedefs.h"

#include "../interfaces/ibinningclass.h"
#include "base_ui_calc.h"

#include "../interfaces/idatawindowstub.h"
#include "../interfaces/idatawindow.h"
#include "../interfaces/idataplot.h"
#include "../support/global.h"

class IPermanentDataStorage;
class ITempDataStorage;
class DataView;

namespace Ui {
class base_view_plot;
}
class base_view_plot;

class TT_EXPORT base_view_plot_datawindow : public IDataWindow
{
    Q_OBJECT

private:
    base_view_plot* owner;

public:
    base_view_plot_datawindow(const base_view_plot_datawindow&) = delete;
    operator=(const base_view_plot_datawindow&) = delete;

    explicit base_view_plot_datawindow(base_view_plot* owner);

    virtual void emit_got_focus(IDataPlot* plot, IBinningClass *binner);
    virtual void emit_plot_deleted(IDataPlot* plot);
    virtual void emit_plot_binner_deleted(IBinningClass* binner);

public slots:
    virtual void clear_data() override;
    virtual void on_new_tempdatastorage(ITempDataStorage*storage) override;
    virtual void show_window() override;
};


class TT_EXPORT base_view_plot : public QDialog, public IDataWindowStub, public IBinningClass
{
    Q_OBJECT

protected:
    virtual Base_ui_calc *init_single_calc(const std::vector<chan_id> & channels,
                    uint64_t bin_width,
                    opaque_ptr ref,
                    bool display_only_initial_data) = 0;
    virtual plot_params get_default_plotparams() = 0;
    virtual double get_def_linewidth() = 0;
    virtual QwtPlotCurve::CurveStyle get_def_pen_style() = 0;
    virtual QString get_x_axis_title() = 0;
    virtual QString get_y_axis_title() = 0;

    void init_calcs();

private:
    Ui::base_view_plot *ui;
    IPermanentDataStorage* file;
    ITempDataStorage* storage;
    std::vector<chan_id> channels;
    bool sum_chans;
    uint64_t bin_width;

    std::vector<Base_ui_calc*> calcs;
    bool display_only_initial_data;

    base_view_plot_datawindow* window;

protected:
    double time_unit;
    DataView* view;
    bool data_displayed;

public:
    base_view_plot(const base_view_plot&) = delete;
    base_view_plot& operator=(const base_view_plot&) = delete;

    explicit base_view_plot(double time_unit, QWidget *parent,
                               ITempDataStorage* data,
                               const std::vector<chan_id>& channels,
                               bool sum_chans,
                               uint64_t bin_size,
                               QString x_title,
                               QString y_title,
                               double linewidth,
                               QwtPlotCurve::CurveStyle curve_style,
                               bool display_only_initial_data);

    ~base_view_plot();

    virtual void on_new_temp_data_storage(ITempDataStorage* storage);
    virtual void ClearData();

    virtual void focusInEvent( QFocusEvent* e);

    virtual IDataWindow* GetDataWindow() override;
    virtual IDataPlot* GetDataPlot() override;
    virtual IBinningClass* GetBinningClass() override;

    virtual uint64_t get_bin_width() const override;
    virtual void on_new_bin_width(uint64_t width) override;
};

#endif // BASE_VIEW_PLOT_H
