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


#ifndef TILE_H
#define TILE_H

#include <QImage>
#include <vector>

#include "palette.h"

class Tile
{
public:
    Tile(Palette *palette);

    static unsigned int number_bpp;//1;2;3;4

    //static Tile* frompalette(Palette *palette,unsigned char index);
    void update_palette(Palette *palette);
    void update_image();
    void empty_image();
    bool is_empty(){return m_is_empty;}

    /*read all the tile in ROM. Returns offset of the next tile*/
    unsigned long read(unsigned char * romdata, unsigned long offset, unsigned long romlength);

    /*read 8 pixels (1 raw) in ROM. Returns number of bytes read*/
    unsigned long read_8pixels(unsigned char * romdata, unsigned long offset, int line);

    static unsigned long row_size(); //one row size in byte
    static unsigned long tile_size(); //one tile size in byte

    QImage image;
    std::vector< std::vector<unsigned char> > data;
protected:
    bool m_is_empty;
};

#endif // TILE_H
