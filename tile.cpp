#include "tile.h"

#include <iostream>

#include "tile3bpp.h"

Tile::Tile(Palette *palette) :image(8,8,QImage::Format_Indexed8)
{
    for (int x=0;x<8;x++)
    {
        data.push_back(std::vector<unsigned char>());
        for (int y=0;y<8;y++)
            data[x].push_back(0);
    }
    update_palette(palette);
    update_image();
}

void Tile::update_palette(Palette *palette)
{
    image.setColorTable( palette->get_colors() );
}

void Tile::update_image()
{
    for (int x=0;x<8;x++)
        for (int y=0;y<8;y++)
        {
            //std::cout<<"Pixel "<<x<<","<<y<<": "<<(int)(data[y][x])<<std::endl;
            image.setPixel(x,y,data[y][x]);
        }

}


unsigned long Tile::read(unsigned char * romdata, unsigned long offset)
{
    for (int i=0;i<8;i++)
        offset+=read_8pixels(romdata,offset,i);
    update_image();
    return offset;
}

/*
Tile* Tile::frompalette(Palette *palette,unsigned char index)
{
    Tile* tile=new Tile3bpp(palette);
    for (int x=0;x<8;x++)
        for (int y=0;y<8;y++)
        {
            tile->data[y][x]=index;
            tile->update_image();
        }
    return tile;
}*/
