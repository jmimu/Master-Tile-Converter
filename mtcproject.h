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

#ifndef MTCPROJECT_H
#define MTCPROJECT_H

#include <string>
#include <fstream>
#include <vector>

#include <QtXml/QDomDocument>
#include <QFile>
#include <QTextStream>

#include "palette.h"
#include "rom.h"


//bookmark on the rom (will be ordered by offset)
class MTCbookmark
{
public:
    MTCbookmark(unsigned long offset,short mode,QString description);
    MTCbookmark(QDomElement &e);
    QDomElement toNode(QDomDocument &d);
    unsigned int get_offset(){return m_offset;}
    short get_mode(){return m_mode;}
    QString get_description(){return m_description;}
protected:
    unsigned int m_offset;
    short m_mode;
    QString m_description;
};


class MTCproject
{
public:
    MTCproject(QString filename="");
    ~MTCproject();

    MTCbookmark* read_project(QString filename);//return start point (or 0 if error)
    bool save_project(QString filename);
    void clear();

    std::vector<Palette*> * getPalettes(){return &m_palettes;}
    std::vector<MTCbookmark*> * getBookmarks(){return &m_bookmarks;}
    MTCbookmark* getBookmark(int i){return m_bookmarks.at(i);}

    bool load_ROM(QString ROMpath,bool is_original_rom=true);
    void save_ROM(QString ROMpath){m_rom->save_ROM(ROMpath.toStdString());m_ROM_filename=ROMpath;}

    void add_bookmark(QString description,unsigned long offset,short mode);
    void add_bookmark(MTCbookmark *b);

    Palette* getPalette(){return m_palettes.at(m_current_palette_index);}
    int getPaletteIndex(){return m_current_palette_index;}
    Palette* setPalette(unsigned int i);
    Palette* addPalette(QString description,long offset);
    Palette* addPalette(QString description,QString filename,bool show_msg=false);
    void setROM(Rom *rom){m_rom=rom;}
    Rom * getROM(){return m_rom;}

    void setOffset(long offset){m_current_offset=offset;}
    long getOffset(){return m_current_offset;}
    void setMode(int mode){m_current_mode=mode;}
    int getMode(){return m_current_mode;}
    QString getFilename(){return m_filename;}
    QString get_original_ROM_filename(){return m_original_ROM_filename;}
    void set_sprite_part_of_palette(bool sprite_part_of_palette){m_sprite_part_of_palette=sprite_part_of_palette;}

private:
    static MTCproject * current_config;
    Rom *m_rom;
    std::vector<Palette*> m_palettes;
    std::vector<MTCbookmark*> m_bookmarks;
    QString m_filename;
    QString m_original_ROM_filename;
    QString m_ROM_filename;

    //just for save and load
    int m_current_palette_index;
    int m_current_mode;
    long m_current_offset;
    bool m_sprite_part_of_palette;
};

#endif // MTCPROJECT_H
