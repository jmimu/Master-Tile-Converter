/*MASTER TILE CONVERTER

Copyright (C) 2012  JM Muller

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "rom.h"

#include <QImage>
#include <QPainter>
#include <QMessageBox>

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
    if (is.fail()) return false;

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
    if (!header.check_TMR_SEGA())
    {
        QMessageBox msgBox;
        msgBox.setText("Bad rom signature (must be \"TMR SEGA\").");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    std::cout<<"Read checksum: "<<std::hex<<header.read_checksum()<<std::endl;
    header.compute_checksum();

    return true;
}

bool Rom::export_BMP(std::string filename,int nbbpp)
{
    int nb_tiles_width=16;
    QImage img(nb_tiles_width*8,12*8,QImage::Format_Indexed8);
    //can't use painter on 8bit images => fill it pixel-by-pixel...
    //define palette (16 or 8 colors, only background or sprites part)
    img.setColorTable( m_palette->get_colors() );
    switch (nbbpp)
    {
    case 1:
        img.setColorCount(4);//to avoid gimp 2 colors mode !
        break;
    case 2:
        img.setColorCount(4);
        break;
    case 3:
        img.setColorCount(8);
        break;
    default:
        img.setColorCount(16);
        break;
    }

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
bool Rom::import_BMP(std::string filename,int nbbpp)
{
    int nb_tiles_width=0;
    int nb_tiles_height=0;
    QImage img(filename.c_str(),"BMP");

    if (img.isNull())
    {
        QMessageBox msgBox;
        msgBox.setText("Problem loading image.");
        msgBox.exec();
        return false;
    }

    if (img.colorCount()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("The picutre must be in indexed colors.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }


    else if (nbbpp==1)
    {
        if (img.colorCount()>4)//4 to avoid gimp 2 colors mode
        {
            QMessageBox msgBox;
            msgBox.setText("Wrong image palette or color profile.\nMaximum palette size is 4.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }
    }
    else if (nbbpp==2)
    {
        if (img.colorCount()>4)
        {
            QMessageBox msgBox;
            msgBox.setText("Wrong image palette or color profile.\nMaximum palette size is 4.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }
    }
    else if (nbbpp==3)
    {
        if (img.colorCount()>8)
        {
            QMessageBox msgBox;
            msgBox.setText("Wrong image palette or color profile.\nMaximum palette size is 8.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }
    }
    else
    {
        if (img.colorCount()>16)
        {
            QMessageBox msgBox;
            msgBox.setText("Wrong image palette or color profile.\nMaximum palette size is 16.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return false;
        }
    }

    if(img.width()%8 != 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Image width must be n*8 pixels.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }

    nb_tiles_width=img.width()/8;
    nb_tiles_height=img.height()/8;

    unsigned char byte1,byte2,byte3,byte4;//what to write

    unsigned long index=m_offset;
    for (int y_tile=0;y_tile<nb_tiles_height;y_tile++)
    {
        for (int x_tile=0;x_tile<nb_tiles_width;x_tile++)
        {
            if (index+Tile::tile_size()>=(unsigned long)romlength)
                break;

            for (int y=0;y<8;y++)
            {
                //depending on number of bpp, write data to rom
                byte1=byte2=byte3=byte4=0;
                if (Tile::number_bpp==1)
                {
                    long shift=7;
                    for (int x=0;x<8;x++)
                    {
                        //std::cout<<"Pixel "<<img.pixelIndex(8*x_tile+x,8*y_tile+y)<<" ";
                        byte1+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>0)%2)<<shift;
                        shift--;
                    }
                    romdata[index++]=byte1;
                    //std::cout<<"\nRaw: "<<(int)byte1<<std::endl;
                }
                else if (Tile::number_bpp==2)
                {
                    long shift=7;
                    for (int x=0;x<8;x++)
                    {
                        //std::cout<<"Pixel "<<img.pixelIndex(8*x_tile+x,8*y_tile+y)<<" ";
                        byte1+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>0)%2)<<shift;
                        byte2+=(((img.pixelIndex(8*x_tile+x,8*y_tile+y))>>1)%2)<<shift;
                        shift--;
                    }
                    romdata[index++]=byte1;
                    romdata[index++]=byte2;
                    //std::cout<<"\nRaw: "<<(int)byte1<<" "<<(int)byte2<<std::endl;
                }
                else if (Tile::number_bpp==3)
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

bool Rom::import_rawdata(std::string filename,unsigned long adress)//<used for compressed tiles
{
    //from http://www.cplusplus.com/reference/iostream/istream/read/
    int data_length;
    unsigned char * data_buffer;

    std::ifstream is;
    is.open (filename.c_str(), std::ios::binary );
    if (is.fail()) return false;

    // get length of file:
    is.seekg (0, std::ios::end);
    data_length = is.tellg();
    is.seekg (0, std::ios::beg);

    // allocate memory:
    data_buffer = new unsigned char [data_length];

    // read data as a block:
    is.read ((char*)data_buffer,data_length);
    is.close();

    std::cout<<"Read "<<data_length<<" bytes in raw data file "<<filename<<"."<<std::endl;

    //import data in romdata
    unsigned long j=adress;
    for (unsigned long i=0;i<data_length;i++)
    {
        romdata[j]=data_buffer[i];
        j++;
    }

    std::cout<<"Raw data imported."<<std::endl;

    delete[] data_buffer;

    return true;
}


bool Rom::set_romdata(unsigned long address,std::vector<unsigned char> *data)
{
    for (unsigned int i=0;i<data->size();i++)
    {
        if (address>=romlength) return false;
        romdata[address]=data->at(i);
        address++;
    }
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
