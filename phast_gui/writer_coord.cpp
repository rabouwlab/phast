#include "writer_coord.h"

#include "interfaces/ifilewriter.h"
#include "support/small_tcspc_io.h"
#include "support/small_tcspc_io2.h"

#include <thread>

template class writer_coord<small_tcspc_io>;
template class writer_coord<small_tcspc_io2>;

template <typename file_writer>
writer_coord<file_writer>::~writer_coord()
{
    this->UnregisterListeners();

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;
        delete wl;
    }
}

template <typename file_writer>
writer_coord<file_writer>::writer_coord(double time_unit,
                           QString device_type,
                           QString base_filename,
                           QString file_path,
                           const std::map<chan_id, chaninfo>&channels,
                           bool write_only_once) :
    time_unit(time_unit),
    base_filename(base_filename),
    channels(channels),
    write_only_once(write_only_once),
    is_done(false),
    storage(nullptr),
    file_path(file_path),
    n_photons_written(0),
    n_bytes_written(0),
    device_type(device_type),
    start_date_time(QDateTime::currentDateTimeUtc())
{
    for (auto pair: this->channels) {
        chan_id c = pair.first;
        chaninfo ci = pair.second;

        QString fn = base_filename + ".c" + QString::number(c);
        file_writer* fw = new file_writer(file_path+fn);

        if (fw->WritesMicroTimesExplicitely() && ci.is_pulses_channel) {
            // We should not write the macro times of this channel,
            // since the file format we are using supports writing out
            // micro times explicitely
            delete fw;
        } else {
            this->writers[c] = new writer_listeners(c, fw, this->write_only_once);
            this->output_filenames[c] = fn;
        }
    }

    this->write_temp_overviewfile();
}

template <typename file_writer>
void writer_coord<file_writer>::RegisterListeners(ITempDataStorage *storage)
{
    if (this->storage != nullptr)
        return;

    this->storage = storage;

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;

        storage->AddListener(wl);
    }
}

template <typename file_writer>
void writer_coord<file_writer>::UnregisterListeners()
{
    this->NumBytesWritten();
    this->NumPhotonsWritten();

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;

        this->storage->RemoveListener(wl);
    }

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;

        while (wl->IsInEvent()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

template <typename file_writer>
void writer_coord<file_writer>::kill_writers()
{
    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;
        delete wl;
    }

    this->writers.clear();
}

template <typename file_writer>
bool writer_coord<file_writer>::IsDone()
{
    if (!this->write_only_once) {
        return this->is_done;
    }

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;

        if (!wl->IsDone())
            return false;
    }

    this->complete_overviewfile();
    this->is_done = true;
    return true;
}

template <typename file_writer>
void writer_coord<file_writer>::Finalize()
{
    if (this->is_done)
        return;

    this->UnregisterListeners();
    this->complete_overviewfile();
    this->kill_writers();
    this->is_done = true;
}

// TODO: write the overview file from the start.
template <typename file_writer>
void writer_coord<file_writer>::write_temp_overviewfile()
{
    FILE* of = fopen((this->file_path + "/" + this->base_filename + "_temp").toStdString().c_str(), "w");

    if (of == nullptr) {
        return; // TODO: emit error msg
    }

    fprintf(of, "Simple Small Time Tagged (V2)\n"
                "# This file was automatically generated, do not alter its contents.\n"
                "\n\n\n\n\n\n");

    fprintf(of, "EXPERIMENT_HEADER\n");

    fprintf(of, "Time_unit_seconds\tdevice_type\texperiment_start_timestamp_UTC\n");
    fprintf(of, "%.15e\t%s\t%s\n", this->time_unit, this->device_type.toStdString().c_str(), this->start_date_time.toString(Qt::ISODate).toStdString().c_str());

    fprintf(of, "\n\n\n\n\n\n");

    fprintf(of, "CHANNEL_HEADER\n");

    fprintf(of, "ChannelID\t"
                "Filename\t"
                "NumPhotons\t"
                "NumOverflows\t"
                "Filesize\t"
                "HardwareSyncDivider\t"
                "AdditionalSyncDivider\t"
                "TotalSyncDivider\t"
                "IsPulsesChannel\t"
                "HasPulsesChannel\t"
                "CorrespondingPulsesChannel\n");

    for (auto pair : this->writers) {
        chan_id channel = pair.first;
        writer_listeners* wl = pair.second;

        fprintf(of,
                    "%llu\t"        // Channel ID
                    "\"%s\"\t"      // Filename
                    "%llu\t"       // NumPhotons
                    "%llu\t"       // NumOverflows
                    "%llu\t"       // Filesize
                    "%llu\t"      // HardwareSyncDivider
                    "%llu\t"        // AdditionalSyncDivider
                    "%llu\t"        // TotalSyncDivider
                    "%i\t"         // IsPulsesChannel
                    "%i\t"         // HasPulsesChannel
                    "%llu\t"         // CorrespondingPulsesChannel
                    "\n",
                channel,
                this->output_filenames.at(channel).toStdString().c_str(),
                0,
                0,
                0,
                this->channels.at(channel).sync_divider,
                this->channels.at(channel).additional_sync_divider,
                this->channels.at(channel).sync_divider * this->channels.at(channel).additional_sync_divider,
                this->channels.at(channel).is_pulses_channel,
                this->channels.at(channel).has_pulses_channel,
                this->channels.at(channel).corresponding_pulses_channel); // Delay time zero for now
    }

    fclose(of);
}

// TODO: write the overview file from the start.
template <typename file_writer>
void writer_coord<file_writer>::complete_overviewfile()
{
    FILE* of = fopen((this->file_path + "/" + this->base_filename).toStdString().c_str(), "w");

    if (of == nullptr) {
        return; // TODO: emit error msg
    }

    fprintf(of, "Simple Small Time Tagged (V2)\n"
                "# This file was automatically generated, do not alter its contents.\n"
                "\n\n\n\n\n\n");

    fprintf(of, "EXPERIMENT_HEADER\n");

    fprintf(of, "Time_unit_seconds\tdevice_type\texperiment_start_timestamp_UTC\n");
    fprintf(of, "%.15e\t%s\t%s\n", this->time_unit, this->device_type.toStdString().c_str(), this->start_date_time.toString(Qt::ISODate).toStdString().c_str());

    fprintf(of, "\n\n\n\n\n\n");

    fprintf(of, "CHANNEL_HEADER\n");

    fprintf(of, "ChannelID\t"
                "Filename\t"
                "NumPhotons\t"
                "NumOverflows\t"
                "Filesize\t"
                "HardwareSyncDivider\t"
                "AdditionalSyncDivider\t"
                "TotalSyncDivider\t"
                "IsPulsesChannel\t"
                "HasPulsesChannel\t"
                "CorrespondingPulsesChannel\n");

    for (auto pair : this->writers) {
        chan_id channel = pair.first;
        writer_listeners* wl = pair.second;

        fprintf(of,
                    "%llu\t"        // Channel ID
                    "\"%s\"\t"      // Filename
                    "%llu\t"       // NumPhotons
                    "%llu\t"       // NumOverflows
                    "%llu\t"       // Filesize
                    "%llu\t"      // HardwareSyncDivider
                    "%llu\t"        // AdditionalSyncDivider
                    "%llu\t"        // TotalSyncDivider
                    "%i\t"         // IsPulsesChannel
                    "%i\t"         // HasPulsesChannel
                    "%llu\t"         // CorrespondingPulsesChannel
                    "\n",
                channel,
                this->output_filenames.at(channel).toStdString().c_str(),
                wl->NumPhotonsWritten(),
                wl->NumOverflows(),
                wl->NumBytesWritten(),
                this->channels.at(channel).sync_divider,
                this->channels.at(channel).additional_sync_divider,
                this->channels.at(channel).sync_divider * this->channels.at(channel).additional_sync_divider,
                this->channels.at(channel).is_pulses_channel,
                this->channels.at(channel).has_pulses_channel,
                this->channels.at(channel).corresponding_pulses_channel); // Delay time zero for now
    }

    fclose(of);

    // Delete temporary overview file
    remove((this->file_path + "/" + this->base_filename + "_temp").toStdString().c_str());
}

template <typename file_writer>
uint64_t writer_coord<file_writer>::NumPhotonsWritten()
{
    if (this->writers.size() == 0) {
        return this->n_photons_written;
    }

    uint64_t ret = 0;

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;
        ret += wl->NumPhotonsWritten();
    }

    this->n_photons_written = ret;

    return ret;
}

template <typename file_writer>
uint64_t writer_coord<file_writer>::NumBytesWritten()
{
    if (this->writers.size() == 0) {
        return this->n_bytes_written;
    }

    uint64_t ret = 0;

    for (auto pair : this->writers) {
        writer_listeners* wl = pair.second;
        ret += wl->NumBytesWritten();
    }

    this->n_bytes_written = ret;

    return ret;
}
