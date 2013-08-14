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
    void create_tiles(long offset);
    long get_offset(){return m_offset;}
    std::vector<Tile*> * get_tiles(){return & m_tiles;}
    bool export_BMP(std::string filename,int nbbpp,long nb_tiles=0);
    bool import_BMP(std::string filename,int nbbpp,bool update_palette=false);
    bool rom_data2asm(std::string filename,long offset,long num_tiles,int tile_bytes);
    long test_decompress_tiles(Rom * origin, long index);//return number of bytes read if ok, 0 if not valid, -1 if out of rom
    long decompress_tiles(Rom * origin, long index);//return number of tiles uncompressed
    long compress_tiles(int nbr_tiles);//return number of bytes used
    bool import_rawdata(std::string filename,long adress);//<used for compressed tiles
    bool save_ROM(std::string filename);
    unsigned char * get_romdata(){return romdata;}
    bool set_romdata(long address,std::vector<unsigned char> *data);//<change a part of the rom data
    long get_romlength(){return romlength;}
    void set_offset(long offset){if (offset<romlength) m_offset=offset;}
    void set_palette(Palette *palette){m_palette=palette;}
    long get_compressed_size(){return compressed_size;}
protected:
    long romlength;
    unsigned char * romdata; // exact data of the ROM

    std::vector<Tile*> m_tiles; //a certain number of tiles extracted from romdata
    Palette *m_palette;

    long m_offset;

    long compressed_size;//only for comressed data

};

#endif // ROM_H
