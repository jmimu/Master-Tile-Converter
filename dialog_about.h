/*MASTER TILE CONVERTER

Copyright (C) 2012  JM Muller

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef DIALOG_ABOUT_H
#define DIALOG_ABOUT_H

#include <QDialog>

namespace Ui {
    class Dialog_About;
}

class Dialog_About : public QDialog {
    Q_OBJECT
public:
    Dialog_About(QWidget *parent = 0);
    ~Dialog_About();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dialog_About *ui;
};

#endif // DIALOG_ABOUT_H
