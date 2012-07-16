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


#include "palette.h"

#include <iostream>
#include <fstream>

#include <math.h>

Palette::Palette() : m_sprites_palette(true)
{
    std::cout<<"Init palette..."<<std::endl;
    /*for (int i=0;i<16;i++)
        colors.append( QColor(4*i,4*i,4*i).rgb() );*/

    /*
set xrange[0:31]
set yrange[0:255]
plot(127+127*cos(pi/16*x))
replot(127+127*cos(pi/16*x+2*pi/3))
replot(127+127*cos(pi/16*x+4*pi/3))
   */
    /*
    int nbr=32;
    for (int i=0;i<nbr;i++)
    {
        unsigned short r,g,b;
        r=127+127*cos(2*3.14159/nbr*i);
        g=127+127*cos(2*3.14159/nbr*i+2*3.14159/3);
        b=127+127*cos(2*3.14159/nbr*i+4*3.14159/3);
        if (i<nbr/2)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,g,b).rgb() );
    }*/


    //default: make Alex Kidd palette
    colors_back.append( QColor(0x0, 0x0, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0x55, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0x0, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0xaa).rgb() );
    colors_back.append( QColor(0xaa, 0x55, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x55, 0x0, 0x0).rgb() );
    colors_back.append( QColor(0x55, 0xff, 0xff).rgb() );
    colors_back.append( QColor(0x0, 0xaa, 0xff).rgb() );
    colors_back.append( QColor(0x0, 0xff, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0xff, 0xff).rgb() );

    colors_sprites.append( QColor(0x0D,0xE6,0xDF).rgb() );//modified from 0x0 0x0 0xff
    colors_sprites.append( QColor(0xff, 0xff, 0xff).rgb() );
    colors_sprites.append( QColor(0x55, 0x55, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0xaa, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0x0, 0xff).rgb() );
    colors_sprites.append( QColor(0x0, 0xff, 0xff).rgb() );
    colors_sprites.append( QColor(0x0, 0xff, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0xff, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0xaa, 0x0).rgb() );
    colors_sprites.append( QColor(0xaa, 0xaa, 0xff).rgb() );
    colors_sprites.append( QColor(0xaa, 0x55, 0xff).rgb() );
    colors_sprites.append( QColor(0x55, 0x0, 0x0).rgb() );//modified from 0xff 0x0 0x0
    colors_sprites.append( QColor(0xaa, 0xaa, 0x0).rgb() );

}


bool Palette::read_from_file(QString fileName)
{
    long pal_length;
    unsigned char * pal_data;

    std::ifstream is;
    is.open (fileName.toStdString().c_str(), std::ios::binary );
    if (is.fail()) return false;

    // get length of file:
    is.seekg (0, std::ios::end);
    pal_length = is.tellg();
    is.seekg (0, std::ios::beg);

    // allocate memory:
    pal_data = new unsigned char [pal_length];

    // read data as a block:
    is.read ((char*)pal_data,pal_length);
    is.close();

    std::cout<<"Palette: read "<<pal_length<<" bytes."<<std::endl;

    colors_back.clear();
    colors_sprites.clear();

    unsigned char r,g,b;
    for (int i=0;i<32;i++)
    {
        b= ((pal_data[i] & 0x30)>>4)* 0x55;
        g= ((pal_data[i] & 0x0C)>>2)* 0x55;
        r= ((pal_data[i] & 0x03)>>0)* 0x55;
        std::cout<<"read: 0x"<<std::hex<<(int)r<<" 0x"<<(int)g<<" 0x"<<(int)b<<std::endl;
        if (i<16)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,g,b).rgb() );
    }


    return true;
}
