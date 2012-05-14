#include "palette.h"

#include <iostream>
#include <fstream>

#include <math.h>

Palette::Palette() : m_sprites_palette(true)
{
    /*for (int i=0;i<16;i++)
        colors.append( QColor(4*i,4*i,4*i).rgb() );*/

    /*
set xrange[0:31]
set yrange[0:255]
plot(127+127*cos(pi/16*x))
replot(127+127*cos(pi/16*x+2*pi/3))
replot(127+127*cos(pi/16*x+4*pi/3))
   */
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
    }



    //default: make Alex Kidd palette
   /* colors.append( QColor(0x00,0x00,0xFF).rgb() );//#alex kidd 3bpp palette (+8 gray for 4bpp)
    colors.append( QColor(0xFF,0xFF,0xFF).rgb() );
    colors.append( QColor(0x55,0x55,0x00).rgb() );
    colors.append( QColor(0xFF,0xAA,0x00).rgb() );
    colors.append( QColor(0xFF,0x00,0x00).rgb() );
    colors.append( QColor(0xAA,0x00,0x00).rgb() );
    colors.append( QColor(0x00,0x00,0x00).rgb() );
    colors.append( QColor(0x00,0x00,0xFF).rgb() );
    colors.append( QColor(0x10,0x10,0x10).rgb() );//only for 4bpp
    colors.append( QColor(0x30,0x30,0x30).rgb() );
    colors.append( QColor(0x50,0x50,0x50).rgb() );
    colors.append( QColor(0x70,0x70,0x70).rgb() );
    colors.append( QColor(0x90,0x90,0x90).rgb() );
    colors.append( QColor(0xb0,0xb0,0xb0).rgb() );
    colors.append( QColor(0xd0,0xd0,0xd0).rgb() );
    colors.append( QColor(0xF0,0xF0,0xF0).rgb() );*/

}


bool Palette::read_from_file(QString fileName)
{
    long pal_length;
    unsigned char * pal_data;

    std::ifstream is;
    is.open (fileName.toStdString().c_str(), std::ios::binary );

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
        if (i<16)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,g,b).rgb() );
    }


    return true;
}
