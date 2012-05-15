#include "rom.h"

#include <iostream>
#include <fstream>

Rom::Rom(Palette * palette) : romlength(0),romdata(0),m_palette(palette),m_offset(0x10341)
{
    std::cout<<"Init ROM..."<<std::endl;
    m_tiles.clear();
    for (int i=0;i<12*16;i++) // number of tiles is fixed
        m_tiles.push_back(new Tile(m_palette));
}

Rom::~Rom()
{
    if (romdata) delete[] romdata;
}

void Rom::create_tiles(unsigned long offset)
{
    if (!romdata) return;

    m_offset=offset;

    long index=m_offset;
    std::vector<Tile*>::iterator it;
    for ( it=m_tiles.begin() ; it < m_tiles.end(); it++ )
    {
        index=(*it)->read(romdata,index,romlength);
    }
}

bool Rom::loadfile(std::string filename)
{
    //from http://www.cplusplus.com/reference/iostream/istream/read/
    std::ifstream is;
    is.open (filename.c_str(), std::ios::binary );

    // get length of file:
    is.seekg (0, std::ios::end);
    romlength = is.tellg();
    is.seekg (0, std::ios::beg);

    // allocate memory:
    romdata = new unsigned char [romlength];

    // read data as a block:
    is.read ((char*)romdata,romlength);
    is.close();

    std::cout<<"Read "<<romlength<<" bytes."<<std::endl;



    return true;
}
