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
    bool export_BMP(std::string filename);
    bool import_BMP(std::string filename);
    bool save_ROM(std::string filename);
    unsigned char * get_romdata(){return romdata;};
    long get_romlength(){return romlength;};
protected:
    long romlength;
    unsigned char * romdata; // exact data of the ROM

    std::vector<Tile*> m_tiles; //a certain number of tiles extracted from romdata
    Palette *m_palette;

    unsigned long m_offset;

};

#endif // ROM_H
