#ifndef READER_COORD_H
#define READER_COORD_H

#include <QString>
#include <map>

#include "support/typedefs.h"
#include "support/chaninfo.h"
#include "support/typedefs.h"

class reader_coord
{
private:
    QString filename;

    double time_unit;
    QString device_descriptor;

    std::map<chan_id,std::vector<photon_event>*> all_data;



    bool read_channel_header(QString path, QString filename,
                             std::map<chan_id,QString>* filenames,
                             std::map<chan_id,chaninfo>* chaninfos);

    std::map<chan_id,chaninfo> channel_infos;

public:
    std::vector<photon_event>* all_raw_data;

    reader_coord(const reader_coord&) = delete;
    operator=(const reader_coord&) = delete;

    explicit reader_coord(QString filename);

    bool Read();

    double TimeUnit() const;
    QString DeviceDescriptor() const;

    const std::map<chan_id, std::vector<photon_event> *> AllData() const;
    const std::map<chan_id,chaninfo>& ChannelInfos() const;
};

#endif // READER_COORD_H
