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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

#include "tile.h"
#include "palette.h"
#include "rom.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    Palette palette;
    Rom rom;

public slots:
    void show_about();
    bool loadROM();
    bool saveROM();
    bool loadPalette();
    bool export_picture();
    bool import_picture();
    bool import_compressed_data();
    void change_palette();
    void update_tiles();
    void enable_offset_button();
    bool apply_offset();
    //void change_offset_scrollbar(int val);

    bool applyHackFile(bool confirm=true);
    bool applyHackFile_fast(){return applyHackFile(false);};

    void move_up1Byte();
    void move_down1Byte();
    void move_up1row();
    void move_down1row();
    void move_up1tile();
    void move_down1tile();
    void move_up16tiles();
    void move_down16tiles();
    void move_up160tiles();
    void move_down160tiles();

    void change_mode();
};

#endif // MAINWINDOW_H
