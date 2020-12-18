#include "reader_coord.h"

#include <QFile>
#include <QFileInfo>
#include <QStringList>

#include <iostream>
#include <fstream>

#include "support/small_tcspc_io_reader.h"

#include <libtimetag/sstt_file2.h>

reader_coord::reader_coord(QString filename) :
    filename(filename),
    time_unit(81e-12),
    all_raw_data(nullptr)
{
}

double reader_coord::TimeUnit() const
{
    return this->time_unit;
}

QString reader_coord::DeviceDescriptor() const
{
    return this->device_descriptor;
}

bool reader_coord::read_channel_header(QString path,
                                       QString filename,
                                       std::map<chan_id, QString> *filenames,
                                       std::map<chan_id, chaninfo> *chaninfos)
{
    std::ifstream file((path + filename).toStdString());
    std::string str;

    // First line should read "Simple Small Time Tagged"

    if (!std::getline(file,str))
        return false;

    QString s = QString::fromStdString(str);

    if (s != "Simple Small Time Tagged") {
        return false;
    }

    bool got_channel_header_start = false;
    bool got_channel_top_header = false;

    std::map<QString,int32_t> table_header;

    while (std::getline(file, str))
    {
        s = QString::fromStdString(str);

        if (s == "CHANNEL_HEADER") {
            got_channel_header_start = true;
            continue;
        }

        if (got_channel_header_start && !got_channel_top_header) {
            QStringList splitted = s.split('\t');

            for (int32_t i = 0; i < splitted.size(); i++) {
                table_header[splitted.at(i)] = i;
            }

            // Test if the channel header makes sense
            if (table_header.count("ChannelID") == 0 ||
                table_header.count("Filename") == 0 ||
                table_header.count("HasMicrotimes") == 0) {
                return false;
            }

            got_channel_top_header = true;

            continue;
        }

        if (got_channel_header_start && got_channel_top_header) {
            if (s == "") {
                // Channel header is done
                return true;
            }

            QStringList splitted = s.split('\t');

            // Test if the splitted stringlist has enough members
            for (auto pair : table_header) {
                int32_t index = pair.second;

                if (index >= splitted.size()) {
                    return false;
                }
            }

            QString channel_filename;

            chaninfo ci;
            ci.ID = splitted.at(table_header.at("ChannelID")).toULongLong();
            channel_filename = splitted.at(table_header.at("Filename"));
            ci.channel_has_microtime = static_cast<bool>(splitted.at(table_header.at("HasMicrotimes")).toInt());
            ci.corresponding_pulses_channel = 0;
            ci.has_pulses_channel = false;
            ci.is_pulses_channel = false;
            ci.num_dependent_chans = 0;
            ci.micro_display_delaytime = 0;

            (*filenames)[ci.ID] = channel_filename;
            (*chaninfos)[ci.ID] = ci;
        }
    }

    return true;
}

bool reader_coord::Read()
{
    QFile qf(this->filename);

    if (!qf.exists())
        return false;

    QFileInfo qfi(this->filename);

    QString filename = qfi.fileName();
    QString abs_path = qfi.absolutePath() + "/";

    // Test if it is a V2 file
    bool v2 = test_is_sstt2_info_file((abs_path+filename).toStdString());

    if (v2) {
        int error_code = 0;
        exp_info_sstt2 exp_info;
        std::vector<channel_info_sstt2> infos = get_sstt2_info((abs_path+filename).toStdString().c_str(), &error_code, &exp_info);

        if (error_code != 0) {
            std::cout << "Error while reading SSTT v2 info file! Error code: " << error_code << std::endl;
            return false;
        }

        this->time_unit = exp_info.time_unit_seconds;
        this->device_descriptor = QString::fromStdString(exp_info.device_type);

        std::map<chan_id, QString> filenames;

        for (channel_info_sstt2 info : infos) {
            chaninfo ci;
            ci.ID = info.ID;

            ci.sync_divider = info.sync_divider;
            ci.additional_sync_divider = info.additional_sync_divider;
            ci.channel_has_microtime = info.has_pulses_channel;
            ci.corresponding_pulses_channel = info.corresponding_pulses_channel;
            ci.has_pulses_channel = info.has_pulses_channel;
            ci.ID = info.ID;
            ci.is_pulses_channel = info.is_pulses_channel;
            ci.micro_display_delaytime = 0;
            ci.num_dependent_chans = 0; // TODO: determine this value?

            filenames[ci.ID] = QString::fromStdString(info.filename);
            this->channel_infos[ci.ID] = ci;
        }

        this->all_raw_data = new std::vector<photon_event>();

        for (auto pair : channel_infos) {
            chan_id chan = pair.first;
            QString fn = filenames.at(chan);

            if (fn.size() < 2)
                continue;

            fn = fn.remove('"');

            std::vector<int64_t> temp_macrotimes;
            error_code = read_data_file_sstt2((abs_path+fn).toStdString(), &temp_macrotimes,0,0,nullptr);

            if (error_code != 0) {
                std::cout << "Error while reading SSTT v2 data file! Error code: " << error_code << std::endl;
                return false;
            }

            for (int64_t t : temp_macrotimes) {
                photon_event e;
                e.channel_number = chan;
                e.macrotime = t;
                e.microtime = 0;

                this->all_raw_data->push_back(e);
            }
        }

        std::sort(this->all_raw_data->begin(), this->all_raw_data->end());
    } else {
        std::map<chan_id, QString> filenames;

        bool success = this->read_channel_header(abs_path, filename, &filenames, &this->channel_infos);

        if (!success) {
            return false;
        }

        for (auto pair : channel_infos) {
            chan_id chan = pair.first;
            QString fn = filenames.at(chan);

            if (fn.size() < 2)
                continue;

            fn = fn.remove('"');

            this->all_data[chan] = new std::vector<photon_event>();

            small_tcspc_io_reader reader(abs_path + fn, chan, this->all_data.at(chan));
        }

        this->time_unit = 81e-12;
        this->device_descriptor = "unknown device";
    }

    return true;
}

// TODO: transfer the data directly to the requested position
// Now we need to copy it around, which requires as lot of memory :-(.
const std::map<chan_id,std::vector<photon_event>*> reader_coord::AllData() const
{
    return this->all_data;
}

const std::map<chan_id,chaninfo>& reader_coord::ChannelInfos() const
{
    return this->channel_infos;
}
