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
#include <QCloseEvent>

#include <vector>

#include "tile.h"
#include "palette.h"
#include "rom.h"
#include "mtcproject.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool currently_showing_decompressed_data(){return current_rom_shown==&decompressed_rom;}

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    MTCproject *m_project;
    Rom real_rom;//the rom from the file
    Rom decompressed_rom;//a rom created by decompressing data
    Rom * current_rom_shown;//which rom is shown

protected:
    void closeEvent(QCloseEvent *event);

public slots:
    void show_about();    
    bool loadROM();
    bool saveROM();
    bool createIPS();
    bool loadMTCproject();
    bool saveMTCproject();
    bool loadPaletteFile();
    bool export_picture();
    bool import_picture();
    bool import_compressed_data();
    void change_palette();
    void add_palette_from_ROM();
    void update_tiles();
    void update_palettes();
    void update_bookmarks();
    void add_bookmark();
    void goto_bookmark(MTCbookmark * bookmark=0);
    void enable_offset_button();
    bool apply_offset();
    void change_offset_scrollbar(int val);

    bool compress_picture();
    bool compress_and_import();
    bool decompress_tiles();//return false if reached end of ROM
    void return_to_real_rom();
    void show_decompressed_data();

    bool palette2asm();
    bool tile2asm();
    bool BMP2asm();


    bool applyHackFile(bool confirm=true);
    bool applyHackFile_fast(){return applyHackFile(false);}

    void move_up1Byte();
    void move_down1Byte();
    void move_up1row();
    void move_down1row();
    void move_up1tile();
    void move_down1tile();
    void move_up16tiles();
    void move_down16tiles();
    void move_up96tiles();
    void move_down96tiles();

    void change_mode();//number of bpp
};

#endif // MAINWINDOW_H
