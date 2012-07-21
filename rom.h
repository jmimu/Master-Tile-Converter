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


#ifndef ROM_H
#define ROM_H

#include <vector>

#include "tile.h"
#include "palette.h"

class Rom
{
public:
    Rom(Palette *palette);
    ~Rom();
    bool loadfile(std::string filename);
    void create_tiles(unsigned long offset);
    unsigned long get_offset(){return m_offset;};
    std::vector<Tile*> * get_tiles(){return & m_tiles;};
    bool export_BMP(std::string filename,int nbbpp);
    bool import_BMP(std::string filename,int nbbpp);
    bool import_rawdata(std::string filename,unsigned long adress);//<used for compressed tiles
    bool save_ROM(std::string filename);
    unsigned char * get_romdata(){return romdata;};
    bool set_romdata(unsigned long address,std::vector<unsigned char> *data);//<change a part of the rom data
    long get_romlength(){return romlength;};
protected:
    long romlength;
    unsigned char * romdata; // exact data of the ROM

    std::vector<Tile*> m_tiles; //a certain number of tiles extracted from romdata
    Palette *m_palette;

    unsigned long m_offset;

};

#endif // ROM_H
