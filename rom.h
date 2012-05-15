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
protected:
    long romlength;
    unsigned char * romdata;

    std::vector<Tile*> m_tiles;
    Palette *m_palette;

    unsigned long m_offset;

};

#endif // ROM_H
