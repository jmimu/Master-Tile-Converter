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

#include "system_SMS.h"
#include "system_unknown.h"

#include "tileswidget.h"

Rom::Rom(Palette * palette) : romlength(0),romdata(0),m_palette(palette),m_offset(0x10341),compressed_size(0),m_system(0)
{
    std::cout<<"Init ROM..."<<std::endl;
    m_tiles.clear();
    for (int i=0;i<NB_TILES_LINES*16;i++) // number of tiles is fixed for uncompressed data
        m_tiles.push_back(new Tile(m_palette));
}

Rom::~Rom()
{
    if (romdata) delete[] romdata;
    m_tiles.clear();
    if (m_system) delete m_system;
}

void Rom::create_tiles(long offset)
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
    if (m_system) delete m_system;
    
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

    //check ROM system
    //search file extension
    unsigned extension_found = filename.find_last_of(".");
    std::string file_extension=filename.substr(extension_found+1);
    std::cout<<"ROM file extension: "<<file_extension<<std::endl;
    if ((file_extension=="sms")||(file_extension=="SMS")
            ||(file_extension=="gg")||(file_extension=="GG"))
    {
        m_system=new System_SMS();
    }else{
        m_system=new System_Unknown();
    }

    m_system->setROM(this);
    if (!m_system->check_ROM())
    {
        QMessageBox msgBox;
        msgBox.setText("Bad rom signature.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    std::cout<<"Read checksum: "<<std::hex<<m_system->read_checksum()<<std::endl;
    m_system->compute_checksum();

    return true;
}

bool Rom::export_BMP(std::string filename,int nbbpp,long nb_tiles)
{
    long nbr_tiles_lines=NB_TILES_LINES;
    int nb_tiles_width=16;
    if (nb_tiles>0)//this is compressed data
    {
       nbr_tiles_lines=(nb_tiles-1)/nb_tiles_width+1;
    }
    QImage img(nb_tiles_width*8,nbr_tiles_lines*8,QImage::Format_Indexed8);
    //can't use painter on 8bit images => fill it pixel-by-pixel...
    //define palette (16 or 8 colors, only background or sprites part)
    img.setColorTable( m_palette->get_colors() );
    switch (nbbpp)
    {
    case 1:
        img.setColorCount(4);//to avoid gimp 2 colors mode that is messy for us!
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


bool Rom::rom_data2asm(std::string filename,long offset,long num_tiles, int tile_bytes)
{
    std::ofstream file;
    file.open (filename.c_str());
    if (file.fail()) return false;

    m_palette->save_to_asm(file);

    file<<"Tiles_Start:\n";
    //organize by tiles
    long index=offset;
    for (long i=0;i<num_tiles;i++)
    {
        file<<".db ";
        for (long j=0;j<tile_bytes;j++)
        {
            file<<QString("$%1").arg(romdata[index],2,16,QLatin1Char('0')).toStdString();
            if (j<tile_bytes-1)
                file<<",";
            index++;
        }
        file<<"\n";
    }
    file<<"Tiles_End:\n";

    file.close();
    return true;
}



//with "Phantasy Star" RLE
//http://www.smspower.org/Development/Compression
//return number of bytes read if ok, 0 if not valid, -1 if out of rom
long Rom::test_decompress_tiles(Rom * origin, long index)
{
    if (index>= origin->get_romlength()) return -1;

    long offset=0;
    long nb_consecutive_identical_bytes=0;
    long nb_consecutive_different_bytes=0;

    long bytes_in_bitplan[4]={0,0,0,0};
    //std::cout<<"Testing data at "<<index<<"..."<<std::endl;
    for (int num_bitplane=0;num_bitplane<4;num_bitplane++)
    {
        if (num_bitplane==1)
        {
            if (bytes_in_bitplan[0]==0)//already false
                break;
            if (bytes_in_bitplan[0]*4 % Tile::tile_size()!=0)
            {
                //std::cout<<"At "<<std::hex<<index<<", found 1 bitplan for "<<bytes_in_bitplan[0]*4.0/Tile::tile_size()<<" tiles. It's not a round number, forget it."<<std::endl;
                break;
            }
        }
        if (num_bitplane==2)
            if (bytes_in_bitplan[0]!=bytes_in_bitplan[1])//already false
                break;
        if (num_bitplane==3)
            if (bytes_in_bitplan[1]!=bytes_in_bitplan[2])//already false
                break;
        //std::cout<<"bitplane "<<num_bitplane<<std::endl;
        //end when out of rom or read a 0 (end of a bitplan)
        while ((index+offset<origin->get_romlength())and((origin->get_romdata())[index+offset]!=0))
        {
            if ((origin->get_romdata())[index+offset]<128)//identical bytes
            {
                nb_consecutive_identical_bytes=(origin->get_romdata())[index+offset];
                //std::cout<<"   found "<<nb_consecutive_identical_bytes<<" identical bytes"<<std::endl;
                bytes_in_bitplan[num_bitplane]+=nb_consecutive_identical_bytes;
                offset+=1+1;
                continue;
            }
            if ((origin->get_romdata())[index+offset]>128)//consecutive different tiles
            {
                nb_consecutive_different_bytes=(origin->get_romdata())[index+offset]-128;
                //std::cout<<"   found "<<nb_consecutive_different_bytes<<" different bytes"<<std::endl;
                bytes_in_bitplan[num_bitplane]+=nb_consecutive_different_bytes;
                offset+=1+nb_consecutive_different_bytes;
                continue;
            }
            if ((origin->get_romdata())[index+offset]==128)//128 has no meaning! => not compressed data
            {
                bytes_in_bitplan[0]=0;
                break;
            }
        }
        offset+=1;
    }
    //std::cout<<"bytes in bitplans: "<<bytes_in_bitplan[0]<<" "<<bytes_in_bitplan[1]<<" "<<bytes_in_bitplan[2]<<" "<<bytes_in_bitplan[3]<<std::endl;
    if ((bytes_in_bitplan[0]>0)&&(bytes_in_bitplan[0]==bytes_in_bitplan[1])
        &&(bytes_in_bitplan[0]==bytes_in_bitplan[2])&&(bytes_in_bitplan[0]==bytes_in_bitplan[3]))
    {    
        std::cout<<"At "<<std::hex<<index<<", found "<<std::dec<<bytes_in_bitplan[0]*4<<" bytes ("<<bytes_in_bitplan[0]*4.0/Tile::tile_size()<<" tiles) compressed into "<<offset<<" bytes."<<std::endl;
        compressed_size=offset;
        if (bytes_in_bitplan[0]*4<Tile::tile_size())
        {
            std::cout<<"Not enought to be a tile"<<std::endl;
            return 0;
        }
        return offset-1;
    }else{
        //std::cout<<"This is not compressed data."<<std::endl;
        return 0;
    }
}


//with "Phantasy Star" RLE
//http://www.smspower.org/Development/Compression
//return number of tiles uncompressed
long Rom::decompress_tiles(Rom * origin, long index)
{
    if (index>=origin->get_romlength()) return -1;

    long offset=0;
    long nb_consecutive_identical_bytes=0;
    long nb_consecutive_different_bytes=0;
    unsigned char the_byte;

    std::vector<unsigned char> decompressed_bitplane[4];

    //std::cout<<"Testing data at "<<index<<"..."<<std::endl;
    for (int num_bitplane=0;num_bitplane<4;num_bitplane++)
    {
        //std::cout<<"bitplane "<<num_bitplane<<std::endl;
        while ((index+offset<origin->get_romlength())and((origin->get_romdata())[index+offset]!=0))
        {
            if ((origin->get_romdata())[index+offset]<128)//identical bytes
            {
                nb_consecutive_identical_bytes=(origin->get_romdata())[index+offset];
                offset++;
                //std::cout<<"   found "<<nb_consecutive_identical_bytes<<" identical bytes"<<std::endl;
                the_byte=(origin->get_romdata())[index+offset];
                offset++;
                for (long i=0;i<nb_consecutive_identical_bytes;i++)
                    decompressed_bitplane[num_bitplane].push_back(the_byte);
                continue;
            }
            if ((origin->get_romdata())[index+offset]>=128)//consecutive different tiles
            {
                nb_consecutive_different_bytes=(origin->get_romdata())[index+offset]-128;
                offset++;
                //std::cout<<"   found "<<nb_consecutive_different_bytes<<" different bytes"<<std::endl;
                for (long i=0;i<nb_consecutive_different_bytes;i++)
                {
                    the_byte=(origin->get_romdata())[index+offset];
                    decompressed_bitplane[num_bitplane].push_back(the_byte);
                    offset++;
                }
                continue;
            }
        }
        offset+=1;
    }

    //rearrange data
    if (romdata) delete[] romdata;
    romlength=decompressed_bitplane[0].size()*4;
    romdata = new unsigned char [romlength];
    long j=0;
    for (unsigned long i=0;i<decompressed_bitplane[0].size();i++)
        for (long b=0;b<4;b++)
        {
            romdata[j]=decompressed_bitplane[b].at(i);
            j++;
        }

    //we create the correct number of tiles
    long nbr_decompressed_tiles=decompressed_bitplane[0].size()*4/Tile::tile_size();
    m_tiles.clear();
    for (int i=0;i<nbr_decompressed_tiles;i++) // number of tiles is fixed for uncompressed data
        m_tiles.push_back(new Tile(m_palette));

    //std::cout<<"Found "<<(long)decompressed_bitplane[0].size()<<" compressed bytes"<<std::endl;
    return nbr_decompressed_tiles;
}


//with "Phantasy Star" RLE
//http://www.smspower.org/Development/Compression
long Rom::compress_tiles(int nbr_tiles)
{
    /*if (romdata) delete[] romdata;
    romlength=0;
    m_tiles.clear();*/

    if (nbr_tiles*4*8>romlength)
    {
        std::cerr<<"Asking to compress more tiles than exists!"<<std::endl;
    }else{
        romlength=nbr_tiles*4*8;
    }

    //compression of romdata into compressed_data
    std::vector<unsigned char> compressed_data;
    //worst case: all is composed by AABAABAABAAB => romlength*4/3+4+4 (have to begin the 4 bitplans and finish them (+4+4))

    for (long bitplan=0;bitplan<4;bitplan++)
    {
         //std::cout<<"Bitplan "<<bitplan<<std::endl;
         long data_index=bitplan;//look for 1 byte over 4
         while (data_index<romlength)
         {
             //look for similar bytes
             long nb_similar_bytes=1;//the first byte can be at the same time in a group of different or similar bytes
             unsigned char the_unique_byte=0;
             long nb_different_bytes=1;
             std::vector<unsigned char> the_diff_bytes;
             //std::cout<<"We are at data_index="<<data_index<<".  read "<<(int)romdata[data_index]<<std::endl;
             for (long i=data_index+4;i<romlength;i+=4)
             {
                 //std::cout<<"   read "<<(int)romdata[i]<<"   ";
                 if (nb_different_bytes>1)//we are in a diff bytes group
                 {
                     if (((romdata[i]==romdata[i-4])&&(i+4<romlength)&&(romdata[i]==romdata[i+4]))   or(nb_different_bytes==127))//found similar bytes, or out of range
                     {
                         //end of group
                         //remove the last one, since it is part of the next block (of similar bytes)
                         nb_different_bytes--;
                         the_diff_bytes.pop_back();
                         i--;
                         //std::cout<<" end of group"<<std::endl;
                         break;
                     }else{
                         nb_different_bytes++;
                         the_diff_bytes.push_back(romdata[i]);
                     }
                 }else if (nb_similar_bytes>1)//we are in a similar bytes group
                 {
                     if ((romdata[i]!=romdata[i-4])or(nb_similar_bytes==127))//found diff bytes, or out of range
                     {
                         //end of group
                         i--;
                         //std::cout<<" end of group"<<std::endl;
                         break;
                     }else{
                         nb_similar_bytes++;
                     }
                 }else{//we are at the beginning of a group
                     //std::cout<<" begin group ";
                     if ((romdata[i]==romdata[i-4])&&(i+4<romlength)&&(romdata[i]==romdata[i+4]))//found similar bytes
                     {
                         nb_different_bytes=0;
                         nb_similar_bytes=2;
                         the_unique_byte=romdata[i];
                         //std::cout<<" similar ";
                     }else{
                         nb_different_bytes=2;
                         nb_similar_bytes=0;
                         the_diff_bytes.push_back(romdata[i-4]);
                         the_diff_bytes.push_back(romdata[i]);
                         //std::cout<<" different from "<<(int)romdata[i-4]<<" ";
                     }

                 }
             }//one block has been analyzed, we record it.
             if (nb_similar_bytes>2)
             {
                 compressed_data.push_back(nb_similar_bytes);
                 compressed_data.push_back(the_unique_byte);
                 data_index+=nb_similar_bytes*4;
                 //std::cout<<"We write "<<(int)nb_similar_bytes<<" "<<(int)the_unique_byte<<std::endl;
             }else{
                 compressed_data.push_back(nb_different_bytes+128);
                 //std::cout<<"We write "<<(int)(nb_different_bytes+128);

                 for (unsigned long j=0;j<the_diff_bytes.size();j++)
                 {
                     compressed_data.push_back(the_diff_bytes.at(j));
                     //std::cout<<" "<<(int)the_diff_bytes.at(j);
                 }
                 //std::cout<<std::endl;
                 data_index+=nb_different_bytes*4;
             }

         }//end of bitplan, write a 0
         //std::cout<<"end of bitplan"<<std::endl;
         compressed_data.push_back(0);
    }//compression finished

    //write compressed data to a file
    std::ofstream os;
    os.open("compr_tmp.dat", std::ios::out | std::ios::binary);
    if (!compressed_data.empty())
        os.write((const char*)(&compressed_data[0]),compressed_data.size() * sizeof(unsigned char));
    os.close();


    std::cout<<"Wrote compressed data file to \"compr_tmp.dat\"."<<std::endl;

    return compressed_data.size();

}

//TODO import format check !
bool Rom::import_BMP(std::string filename,int nbbpp,bool update_palette)
{
    int nb_tiles_width=0;
    int nb_tiles_height=0;
    QImage img(filename.c_str());

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
        msgBox.setText("The picture must be in indexed colors.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }


    else if (nbbpp==1)
    {
        if (img.colorCount()>4)//4 to avoid gimp 2 colors mode
        {
            QMessageBox msgBox;
            msgBox.setText("Wrong image palette or MTC color profile.\nMaximum palette size is 4.");
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
            msgBox.setText("Wrong image palette or MTC color profile.\nMaximum palette size is 4.");
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
            msgBox.setText("Wrong image palette or MTC color profile.\nMaximum palette size is 8.");
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
            msgBox.setText("Wrong image palette or MTC color profile.\nMaximum palette size is 16.");
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

    //Tile::number_bpp=nbbpp;

    nb_tiles_width=img.width()/8;
    nb_tiles_height=img.height()/8;

    std::cout<<"Found "<<nb_tiles_width<<"x"<<nb_tiles_height<<" tiles in BMP"<<std::endl;

    //if the rom is empty, we just want to create data from BMP:
    if (romlength==0)
    {
        romlength=nb_tiles_width*4*8*nb_tiles_height;
        if (romdata) delete[] romdata;
        romdata = new unsigned char [romlength];
        m_offset=0;
    }
    
    if (update_palette) //get palette from BMP and convert it to SMS palette
    {
        m_palette->read_from_image(&img,true);
    }


    unsigned char byte1,byte2,byte3,byte4;//what to write

    unsigned long index=m_offset;
    for (int y_tile=0;y_tile<nb_tiles_height;y_tile++)
    {
        for (int x_tile=0;x_tile<nb_tiles_width;x_tile++)
        {
            if (index+Tile::tile_size()>romlength)
                break;

            for (int y=0;y<8;y++)
            {
                //depending on number of bpp, write data to rom
                byte1=byte2=byte3=byte4=0;
                if (nbbpp==1)
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
                else if (nbbpp==2)
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
                else if (nbbpp==3)
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
                        //std::cout<<"BMP byte: "<<img.pixelIndex(8*x_tile+x,8*y_tile+y)<<"   ";
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
                    //std::cout<<" => "<<(int)byte1<<" "<<(int)byte2<<" "<<(int)byte3<<" "<<(int)byte4<<std::endl;
                }
            }
        }
    }
    create_tiles(m_offset);
    
    //correct checksum on-the-fly
    
    
    return true;
}

bool Rom::import_rawdata(std::string filename,long adress)//<used for compressed tiles
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

    if (adress+data_length>=romlength)
    {
        std::cout<<"Error: not enought space in ROM at this adress for this raw data file."<<std::endl;
        return false;
    }
    //import data in romdata
    long j=adress;
    for (long i=0;i<data_length;i++)
    {
        romdata[j]=data_buffer[i];
        j++;
    }

    std::cout<<"Raw data imported."<<std::endl;
    create_tiles(m_offset);

    delete[] data_buffer;

    return true;
}


bool Rom::set_romdata(long address,std::vector<unsigned char> *data)
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
    m_system->fix_checksum();

    //from http://www.cplusplus.com/reference/iostream/istream/read/
    std::ofstream os;
    os.open (filename.c_str(), std::ofstream::binary );

    os.write ((char*)romdata,romlength);
    os.close();

    std::cout<<"Wrote ROM file: "<<filename<<"."<<std::endl;
    return true;
}

bool Rom::createIPS(std::string original_rom_filename,std::string ips_filename)
{
    //at first we re-compute the new checksum of current rom
    m_system->fix_checksum();


    unsigned char * original_romdata;
    unsigned long original_romlength=0;

    //re-read original rom
    std::ifstream is;
    is.open (original_rom_filename.c_str(), std::ios::binary );
    if (is.fail()) return false;
    // get length of file:
    is.seekg (0, std::ios::end);
    original_romlength = is.tellg();
    is.seekg (0, std::ios::beg);
    // allocate memory:
    original_romdata = new unsigned char [original_romlength];
    // read data as a block:
    is.read ((char*)original_romdata,original_romlength);
    is.close();

    std::vector<unsigned char> ips_data;
    ips_data.push_back('P');
    ips_data.push_back('A');
    ips_data.push_back('T');
    ips_data.push_back('C');
    ips_data.push_back('H');
    for (unsigned long i=0;i<romlength;i++)
    {
        if (romdata[i]==original_romdata[i])
            continue;
        //std::cout<<"Diff at offset "<<std::hex<<i<<std::endl;
        unsigned long patch_start_offset=i;
        std::vector<unsigned  char> patch_data;
        while ((i<romlength)&&(romdata[i]!=original_romdata[i]))
        {
            //std::cout<<"Diff: "<<std::hex<<(unsigned char)romdata[i]<<" != "<<(unsigned char)original_romdata[i]<<std::endl;
            patch_data.push_back(romdata[i]);
            i++;
        }
        if (i>=romlength)
            break;
        //std::cout<<"add "<<std::hex<<patch_data.size()<<" bytes"<<std::endl;
        ips_data.push_back((patch_start_offset & 0x00FF0000)>>16);
        ips_data.push_back((patch_start_offset & 0x0000FF00)>>8);
        ips_data.push_back((patch_start_offset & 0x000000FF));
        ips_data.push_back((patch_data.size() & 0xFF00)>>8);
        ips_data.push_back((patch_data.size() & 0x00FF));
        for (unsigned long j=0;j<patch_data.size();j++)
        {
            ips_data.push_back(patch_data[j]);
        }
    }
    ips_data.push_back('E');
    ips_data.push_back('O');
    ips_data.push_back('F');

    //write ips file
    std::ofstream os;
    os.open (ips_filename.c_str(), std::ofstream::binary );
    os.write ((char*)&ips_data[0],ips_data.size());
    os.close();
}

