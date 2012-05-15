#ifndef TILE3BPP_H
#define TILE3BPP_H

#include <vector>

#include "tile.h"

class Tile3bpp : public Tile
{
public:
    Tile3bpp(Palette *palette);
    virtual unsigned long read_8pixels(unsigned char * romdata, unsigned long offset, int line);
    virtual unsigned long row_size();
    virtual unsigned long tile_size();
};

#endif // TILE3BPP_H
