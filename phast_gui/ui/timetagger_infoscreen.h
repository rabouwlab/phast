/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef TIMETAGGER_INFOSCREEN_H
#define TIMETAGGER_INFOSCREEN_H

#include <QDialog>

namespace Ui {
class timetagger_infoscreen;
}

class timetagger_infoscreen : public QDialog
{
    Q_OBJECT

public:
    timetagger_infoscreen(const timetagger_infoscreen&) = delete;
    timetagger_infoscreen& operator=(const timetagger_infoscreen&) = delete;

    explicit timetagger_infoscreen(QWidget *parent, bool connected, QString dev_type, double timeunit_seconds);
    ~timetagger_infoscreen();

private:
    Ui::timetagger_infoscreen *ui;
};

#endif // TIMETAGGER_INFOSCREEN_H
