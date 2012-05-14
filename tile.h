#ifndef TILE_H
#define TILE_H

#include <QImage>
#include <vector>

#include "palette.h"

class Tile
{
public:
    Tile(Palette *palette);
    //static Tile* frompalette(Palette *palette,unsigned char index);
    void update_palette(Palette *palette);
    void update_image();

    /*read all the tile in ROM. Returns offset of the next tile*/
    unsigned long read(unsigned char * romdata, unsigned long offset);

    /*read 8 pixels (1 raw) in ROM. Returns number of bytes read*/
    virtual unsigned long read_8pixels(unsigned char * romdata, unsigned long offset, int line)=0;

    QImage image;
    std::vector< std::vector<unsigned char> > data;
};

#endif // TILE_H
