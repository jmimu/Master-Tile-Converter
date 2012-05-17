#include "rom.h"

#include <QImage>
#include <QPainter>

#include <iostream>
#include <fstream>

#include "romheader.h"

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
    if (romdata) delete[] romdata;
    
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

    ROMHeader header(this);
    header.check_TMR_SEGA();
    std::cout<<"Read checksum: "<<std::hex<<header.read_checksum()<<std::endl;
    header.compute_checksum();

    return true;
}

bool Rom::export_BMP(std::string filename)
{
    int nb_tiles_width=16;
    QImage img(nb_tiles_width*8,12*8,QImage::Format_Indexed8);
    //can't use painter on 8bit images => fill it pixel-by-pixel...
    //define palette (16 or 8 colors, only background or sprites part)
    img.setColorTable( m_palette->get_colors() );

    //copy pixels for each tile
    long x_tile=0;
    long y_tile=0;
    std::vector<Tile*>::iterator it;
    for ( it=m_tiles.begin() ; it < m_tiles.end(); it++ )
    {
        if (!(*it)->is_empty())
        {
            for (int x=0;x<8;x++)
                for (int y=0;y<8;y++)
                    img.setPixel(x_tile*8+x , y_tile*8+y, (*it)->data[y][x]);
        }
        x_tile++;
        if (x_tile>=nb_tiles_width)
        {
            x_tile=0;
            y_tile++;
        }
    }

    return img.save(filename.c_str(),"BMP");
}


//TODO import format check !
bool Rom::import_BMP(std::string filename)
{
    int nb_tiles_width=0;
    int nb_tiles_height=0;
    QImage img(filename.c_str(),"BMP");
    nb_tiles_width=img.width()/8;
    nb_tiles_height=img.height()/8;

    unsigned char byte1,byte2,byte3,byte4;//what to write

    unsigned long index=m_offset;
    for (int y_tile=0;y_tile<nb_tiles_height;y_tile++)
    {
        for (int x_tile=0;x_tile<nb_tiles_width;x_tile++)
        {

            for (int y=0;y<8;y++)
            {
                //depending on 3bpp or 4 bpp, write data to rom
                byte1=byte2=byte3=byte4=0;
                if (Tile::is3bpp)
                {
                    long shift=7;
                    for (int x=0;x<8;x++)
                    {
                        //std::cout<<"Pixel "<<img.pixelIndex(8*x_tile+x,8*y_tile+y)<<" ";
                        byte1+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>0)%2)<<shift;
                        byte2+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>1)%2)<<shift;
                        byte3+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>2)%2)<<shift;
                        shift--;
                    }
                    romdata[index++]=byte1;
                    romdata[index++]=byte2;
                    romdata[index++]=byte3;
                    //std::cout<<"\nRaw: "<<(int)byte1<<" "<<(int)byte2<<" "<<(int)byte3<<std::endl;
                }else{
                    long shift=7;
                    for (int x=0;x<8;x++)
                    {
                        byte1+=((img.pixelIndex(8*x_tile+x,8*y_tile+y)>>0)%2)<<shift;
                        byte2+=((img.pixelIndex(8*x_tile+x,8*y_tile+y)>>1)%2)<<shift;
                        byte3+=((img.pixelIndex(8*x_tile+x,8*y_tile+y)>>2)%2)<<shift;
                        byte4+=((img.pixelIndex(8*x_tile+x,8*y_tile+y)>>3)%2)<<shift;
                        shift--;
                    }
                    romdata[index++]=byte1;
                    romdata[index++]=byte2;
                    romdata[index++]=byte3;
                    romdata[index++]=byte4;
                }
            }
        }
    }
    //std::cout<<"Total wrote: "<<index-m_offset<<std::endl;
    create_tiles(m_offset);
    
    //correct checksum on-the-fly
    
    
    return true;
}


bool Rom::save_ROM(std::string filename)
{
    ROMHeader header(this);
    header.fix_checksum();

    //from http://www.cplusplus.com/reference/iostream/istream/read/
    std::ofstream os;
    os.open (filename.c_str(), std::ofstream::binary );

    os.write ((char*)romdata,romlength);
    os.close();

    std::cout<<"Wrote ROM file: "<<filename<<"."<<std::endl;
    return true;
}
