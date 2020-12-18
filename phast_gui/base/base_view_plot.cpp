/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#include "base_view_plot.h"
#include "ui_base_view_plot.h"

#include "../interfaces/itempdatastorage.h"

#include <iostream>

base_view_plot_datawindow::base_view_plot_datawindow(base_view_plot* owner) :
owner(owner)
{
}

void base_view_plot_datawindow::emit_got_focus(IDataPlot* plot,
                                               IBinningClass* binner)
{
    emit got_focus(plot);
    emit got_binner_focus(binner);
}

void base_view_plot_datawindow::emit_plot_deleted(IDataPlot* plot)
{
    emit plot_deleted(plot);
}

void base_view_plot_datawindow::emit_plot_binner_deleted(IBinningClass* binner)
{
    emit plot_binner_deleted(binner);
}

void base_view_plot_datawindow::clear_data()
{
    this->owner->ClearData(); // TODO: fix crash originating here.
}

void base_view_plot_datawindow::on_new_tempdatastorage(ITempDataStorage* storage)
{
    this->owner->on_new_temp_data_storage(storage);
}

void base_view_plot_datawindow::show_window()
{
    this->owner->show();
}

// --

base_view_plot::base_view_plot(double time_unit,
                               QWidget *parent,
                             ITempDataStorage *data,
                             const std::vector<chan_id> &channels,
                             bool sum_chans,
                             uint64_t bin_size,
                             QString x_title,
                             QString y_title,
                             double linewidth,
                             QwtPlotCurve::CurveStyle curve_style,
                           bool display_only_initial_data) :
    QDialog(parent),
    ui(new Ui::base_view_plot),    
    file(nullptr),
    storage(data),
    bin_width(bin_size),
    calcs(),    
    channels(channels),
    sum_chans(sum_chans),
    display_only_initial_data(display_only_initial_data),    
    window(new base_view_plot_datawindow(this)),
    time_unit(time_unit),
    view(nullptr),
    data_displayed(false)
{
    this->setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    this->setFocusPolicy(Qt::StrongFocus);

    this->setWindowTitle("");

    this->ui->qwtPlot->setAxisTitle(QwtPlot::xBottom, x_title);
    this->ui->qwtPlot->setAxisTitle(QwtPlot::yLeft, y_title);

    this->move(0, 100);

    this->view = new DataView(this->ui->qwtPlot, linewidth, curve_style);
}

void base_view_plot::init_calcs()
{
    // Initialize the calculators
    if (this->sum_chans) {
        Base_ui_calc* calc = this->init_single_calc(this->channels, this->bin_width, 0, this->display_only_initial_data);

        this->calcs.push_back(calc);
        this->QObject::connect(calc, &Base_ui_calc::new_data, this->view, &DataView::on_new_data, Qt::QueuedConnection);

        this->storage->AddListener(calc);
    } else {
        for (uint64_t i = 0; i < channels.size(); i++) {
            std::vector<chan_id> c;
            c.push_back(channels[i]);

            Base_ui_calc* calc = this->init_single_calc(c, this->bin_width, i, this->display_only_initial_data);

            this->calcs.push_back(calc);
            connect(calc, &Base_ui_calc::new_data, this->view, &DataView::on_new_data, Qt::QueuedConnection);

            this->storage->AddListener(calc);
        }
    }
}

void base_view_plot::focusInEvent(QFocusEvent *e)
{
    QDialog::focusInEvent(e);
    this->window->emit_got_focus(this->view, this);
}

void base_view_plot::ClearData()
{
    for (Base_ui_calc* l : this->calcs) {
        l->clear_data();

        this->data_displayed = false;
    }
}

void base_view_plot::on_new_temp_data_storage(ITempDataStorage *storage)
{
    this->storage = storage;

    if (storage == nullptr)
        return;

    for (Base_ui_calc* l : this->calcs) {
        l->clear_data();

        this->data_displayed = false;

        storage->AddListener(l);
    }
}

base_view_plot::~base_view_plot()
{
    this->window->emit_plot_deleted(this->view);
    this->window->emit_plot_binner_deleted(this);

    if (this->storage != nullptr) {
        for (uint64_t i = 0; i < this->calcs.size(); i++) {
            // TODO: somehow test if storage is still alive...
            this->storage->RemoveListener(this->calcs[i]);
            delete this->calcs[i];
        }
    }

    delete this->window;
    delete this->view;
    delete ui;
}

void base_view_plot::on_new_bin_width(uint64_t width)
{
    if (width > 0) {
        this->bin_width = width;

        for (Base_ui_calc* c : this->calcs) {
            c->on_new_bin_width(width);
        }
    }
}

uint64_t base_view_plot::get_bin_width() const
{
    return this->bin_width;
}

IDataWindow* base_view_plot::GetDataWindow()
{
    return this->window;
}

IDataPlot* base_view_plot::GetDataPlot()
{
    return this->view;
}

IBinningClass* base_view_plot::GetBinningClass()
{
    return this;
}
