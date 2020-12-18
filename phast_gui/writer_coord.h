#ifndef WRITER_COORD_H
#define WRITER_COORD_H

#include "impl/writer_listeners.h"

#include "support/typedefs.h"

#include "interfaces/itempdatastorage.h"
#include "support/chaninfo.h"

#include <QDateTime>

template <typename file_writer>
class writer_coord
{
private:
    double time_unit;
    QString base_filename;
    std::map<chan_id, chaninfo> channels;
    std::map<chan_id, writer_listeners*> writers;
    bool write_only_once;
    bool is_done;

    std::map<chan_id, QString> output_filenames;

    void complete_overviewfile();

    ITempDataStorage* storage;
    QString file_path;

    uint64_t n_photons_written;
    uint64_t n_bytes_written;
    QString device_type;

    QDateTime start_date_time;

    void write_temp_overviewfile();
    void kill_writers();
public:
    writer_coord(const writer_coord&) = delete;
    writer_coord& operator=(const writer_coord&) = delete;

    ~writer_coord();
    writer_coord(double time_unit,
                 QString device_type,
                 QString base_filename,
                 QString file_path,
                 const std::map<chan_id, chaninfo>& channels,
                 bool write_only_once);

    void RegisterListeners(ITempDataStorage* storage);
    void UnregisterListeners();

    bool IsDone();
    void Finalize();

    uint64_t NumPhotonsWritten();
    uint64_t NumBytesWritten();
};

#endif // WRITER_COORD_H
