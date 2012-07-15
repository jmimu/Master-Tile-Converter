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


#include "tile.h"

#include <iostream>

unsigned int Tile::number_bpp=3;

Tile::Tile(Palette *palette) :image(8,8,QImage::Format_Indexed8), m_is_empty(true)
{
    for (int x=0;x<8;x++)
    {
        data.push_back(std::vector<unsigned char>());
        for (int y=0;y<8;y++)
            data[x].push_back(0);
    }
    update_palette(palette);
}

void Tile::update_palette(Palette *palette)
{
    image.setColorTable( palette->get_colors() );
}

void Tile::update_image()
{
    m_is_empty=false;
    for (int x=0;x<8;x++)
        for (int y=0;y<8;y++)
        {
            //std::cout<<"Pixel "<<x<<","<<y<<": "<<(int)(data[y][x])<<std::endl;
            image.setPixel(x,y,data[y][x]);
        }

}

void Tile::empty_image()
{
    m_is_empty=true;
    for (int x=0;x<8;x++)
        for (int y=0;y<8;y++)
            image.setPixel(x,y,0);
}

unsigned long Tile::read(unsigned char * romdata, unsigned long offset, unsigned long romlength)
{
    if (offset+tile_size()<=romlength)
    {
        for (int i=0;i<8;i++)
            offset+=read_8pixels(romdata,offset,i);
        update_image();
    }else{
        empty_image();
    }
    return offset;
}



unsigned long Tile::read_8pixels(unsigned char * romdata, unsigned long offset, int line)
{
    for (int i=0;i<8;i++)
        data[line][i]=0;

    int nbr_bytes;
    nbr_bytes=number_bpp;

    int shift=0;
    for (int i=0;i<nbr_bytes;i++)
    {
        //std::cout<<"ROM data: "<<(int)(romdata[offset+i])<<std::endl;
        data[line][0]+=( (romdata[offset+i]/128)%2 ) << shift;
        data[line][1]+=( (romdata[offset+i]/64)%2 ) << shift;
        data[line][2]+=( (romdata[offset+i]/32)%2 ) << shift;
        data[line][3]+=( (romdata[offset+i]/16)%2 ) << shift;
        data[line][4]+=( (romdata[offset+i]/8)%2 ) << shift;
        data[line][5]+=( (romdata[offset+i]/4)%2 ) << shift;
        data[line][6]+=( (romdata[offset+i]/2)%2 ) << shift;
        data[line][7]+=( (romdata[offset+i]/1)%2 ) << shift;
        shift++;
    }
    return nbr_bytes;
}

unsigned long Tile::row_size()
{
    return number_bpp;
}

unsigned long Tile::tile_size()
{
    return number_bpp*8;
}
