#ifndef TILE_H
#define TILE_H

#include <QImage>
#include <vector>

#include "palette.h"

class Tile
{
public:
    Tile(Palette *palette);

    static bool is3bpp;

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
