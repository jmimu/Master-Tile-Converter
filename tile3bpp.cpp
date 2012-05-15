#include "tile3bpp.h"

#include <iostream>

Tile3bpp::Tile3bpp(Palette *palette) : Tile(palette)
{
}


unsigned long Tile3bpp::read_8pixels(unsigned char * romdata, unsigned long offset, int line)
{
    for (int i=0;i<8;i++)
        data[line][i]=0;

    int shift=1;

    for (int i=0;i<3;i++)
    {
        //std::cout<<"ROM data: "<<(int)(romdata[offset+i])<<std::endl;
        data[line][0]+=( (romdata[offset+i]/128)%2 ) * shift;
        data[line][1]+=( (romdata[offset+i]/64)%2 ) * shift;
        data[line][2]+=( (romdata[offset+i]/32)%2 ) * shift;
        data[line][3]+=( (romdata[offset+i]/16)%2 ) * shift;
        data[line][4]+=( (romdata[offset+i]/8)%2 ) * shift;
        data[line][5]+=( (romdata[offset+i]/4)%2 ) * shift;
        data[line][6]+=( (romdata[offset+i]/2)%2 ) * shift;
        data[line][7]+=( (romdata[offset+i]/1)%2 ) * shift;
        shift*=2;
    }
    return 3;
}

unsigned long Tile3bpp::row_size()
{
    return 3;
}

unsigned long Tile3bpp::tile_size()
{
    return 3*8;
}
