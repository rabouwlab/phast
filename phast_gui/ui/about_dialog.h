/* Copyright (c) 2020 Stijn Hinterding, Utrecht University
 * This sofware is licensed under the MIT license (see the LICENSE file)	
*/

#ifndef ABOUT_DIALOG_H
#define ABOUT_DIALOG_H

#include <QDialog>

namespace Ui {
class about_dialog;
}

class about_dialog : public QDialog
{
    Q_OBJECT

public:
    about_dialog(const about_dialog&) = delete;
    about_dialog& operator=(const about_dialog&) = delete;
    explicit about_dialog(QWidget *parent = nullptr);
    ~about_dialog();

private:
    Ui::about_dialog *ui;
};

#endif // ABOUT_DIALOG_H
