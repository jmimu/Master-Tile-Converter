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


#include "palette.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <QMessageBox>
#include <math.h>

Palette::Palette(QString description) : m_sprites_palette(true),m_description(description),from_filename("?"),m_correct_bytes_in_rom(-1)
{
    std::cout<<"Init palette..."<<std::endl;
    /*for (int i=0;i<16;i++)
        colors.append( QColor(4*i,4*i,4*i).rgb() );*/

    /*
set xrange[0:31]
set yrange[0:255]
plot(127+127*cos(pi/16*x))
replot(127+127*cos(pi/16*x+2*pi/3))
replot(127+127*cos(pi/16*x+4*pi/3))
   */

    /*
    int nbr=32;
    for (int i=0;i<nbr;i++)
    {
        unsigned short r,g,b;
        r=127+127*cos(2*3.14159/nbr*i*7);
        g=127+127*cos(2*3.14159/nbr*i*7+2*3.14159/3);
        b=127+127*cos(2*3.14159/nbr*i*7+4*3.14159/3);
        if (i<nbr/2)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,r,r).rgb() );
    }*/


    //default: make Alex Kidd palette
    colors_back.append( QColor(0x0, 0x0, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0x55, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0xff).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0x0, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xff, 0xaa).rgb() );
    colors_back.append( QColor(0xaa, 0x55, 0x0).rgb() );
    colors_back.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x55, 0x0, 0x0).rgb() );
    colors_back.append( QColor(0x55, 0xff, 0xff).rgb() );
    colors_back.append( QColor(0x0, 0xaa, 0xff).rgb() );
    colors_back.append( QColor(0x0, 0xff, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0xaa, 0x0).rgb() );
    colors_back.append( QColor(0x0, 0xff, 0xff).rgb() );

    colors_sprites.append( QColor(0x0D,0xE6,0xDF).rgb() );//modified from 0x0 0x0 0xff
    colors_sprites.append( QColor(0xff, 0xff, 0xff).rgb() );
    colors_sprites.append( QColor(0x55, 0x55, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0xaa, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0xaa, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0x0, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0x0, 0xff).rgb() );
    colors_sprites.append( QColor(0x0, 0xff, 0xff).rgb() );
    colors_sprites.append( QColor(0x0, 0xff, 0x0).rgb() );
    colors_sprites.append( QColor(0xff, 0xff, 0x0).rgb() );
    colors_sprites.append( QColor(0x0, 0xaa, 0x0).rgb() );
    colors_sprites.append( QColor(0xaa, 0xaa, 0xff).rgb() );
    colors_sprites.append( QColor(0xaa, 0x55, 0xff).rgb() );
    colors_sprites.append( QColor(0x55, 0x0, 0x0).rgb() );//modified from 0xff 0x0 0x0
    colors_sprites.append( QColor(0xaa, 0xaa, 0x0).rgb() );

}

int Palette::try_to_find_in_rom(QString fileName, unsigned char *romdata, long romlength,bool show_msg)
{
    from_offset=-1;//we will try to find that...

    std::cout<<"Try to find palette "<<fileName.toStdString()<<" in ROM\n";
    long pal_length;
    unsigned char * pal_data;

    std::ifstream is;
    is.open (fileName.toStdString().c_str(), std::ios::binary );
    if (is.fail()) return -2;
    m_correct_bytes_in_rom=-1;

    // get length of file:
    is.seekg (0, std::ios::end);
    pal_length = is.tellg();
    is.seekg (0, std::ios::beg);

    // allocate memory:
    pal_data = new unsigned char [pal_length];

    // read data as a block:
    is.read ((char*)pal_data,pal_length);
    is.close();

    std::cout<<"Palette: read 0x"<<pal_length<<" bytes."<<std::endl;

    //try to find the data in rom
    int correct_bytes_in_rom=0;
    int best_correct_bytes_in_rom=0;
    std::vector<int> best_offsets;//offsets where we have best_correct_bytes_in_rom

    long in_rom_at;

    for (in_rom_at=0;in_rom_at<romlength-pal_length;in_rom_at++)
    {
        //std::cout<<"Try rom at "<<in_rom_at<<"\n";
        long i;
        correct_bytes_in_rom=0;
        for (i=0;i<pal_length;i++)
        {
            //std::cout<<"---- "<<in_rom_at+i<<" "<<i<<" -> "<<(int)romdata[in_rom_at+i]<<" ? "<<(int)pal_data[i]<<"\n";
            if (romdata[in_rom_at+i]==pal_data[i])
                correct_bytes_in_rom++;
            if ((i>3) && (correct_bytes_in_rom==0))//if the 3 first bytes are not correct, don't bother searching at that offset
                break;
            if ((i-correct_bytes_in_rom>pal_length-best_correct_bytes_in_rom))
                break;
        }
        if (correct_bytes_in_rom>best_correct_bytes_in_rom)
        {
            //std::cout<<"At "<<in_rom_at<<": "<<correct_bytes_in_rom<<" correct bytes."<<std::endl;
            best_correct_bytes_in_rom=correct_bytes_in_rom;
            best_offsets.clear();
            best_offsets.push_back(in_rom_at);
            continue;
        }
        if (correct_bytes_in_rom==best_correct_bytes_in_rom)
        {
            best_offsets.push_back(in_rom_at);
            continue;
        }
    }
    if (best_correct_bytes_in_rom>0)
    {
        QString info=QString("List of offsets with %1/32 correct bytes:\n").arg(best_correct_bytes_in_rom,0,10);
        for (unsigned int i=0;i<best_offsets.size();i++)
        {
            info+=QString("- 0x%1\n").arg(best_offsets.at(i),0,16);
        }
        std::cout<<info.toStdString()<<std::endl;

        if (best_offsets.size()>0)
        {
            from_offset=best_offsets.at(0);
        }
        m_correct_bytes_in_rom=best_correct_bytes_in_rom;

        if (show_msg)
        {
            QMessageBox::information(NULL, "Palette", info);
        }

        return best_correct_bytes_in_rom;
    }

    return -1;
}

bool Palette::read_from_file(QString fileName)
{
    long pal_length;
    unsigned char * pal_data;

    std::ifstream is;
    is.open (fileName.toStdString().c_str(), std::ios::binary );
    if (is.fail()) return false;

    // get length of file:
    is.seekg (0, std::ios::end);
    pal_length = is.tellg();
    is.seekg (0, std::ios::beg);

    if (pal_length!=32)
    {
        std::cout<<"Error: "<<fileName.toStdString()<<" is not 32 bytes palette."<<std::endl;
        return false;
    }

    // allocate memory:
    pal_data = new unsigned char [pal_length];

    // read data as a block:
    is.read ((char*)pal_data,pal_length);
    is.close();

    std::cout<<"Palette: read 0x"<<pal_length<<" bytes."<<std::endl;

    colors_back.clear();
    colors_sprites.clear();

    unsigned char r,g,b;
    for (int i=0;i<pal_length;i++)
    {
        b= ((pal_data[i] & 0x30)>>4)* 0x55;
        g= ((pal_data[i] & 0x0C)>>2)* 0x55;
        r= ((pal_data[i] & 0x03)>>0)* 0x55;
        //std::cout<<"read: 0x"<<std::hex<<(int)r<<" 0x"<<(int)g<<" 0x"<<(int)b<<std::endl;
        if (i<16)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,g,b).rgb() );
    }

    from_filename=fileName;

    return true;
}


bool Palette::save_to_asm(QString fileName)
{
    std::ofstream file;
    file.open (fileName.toStdString().c_str());
    if (file.fail()) return false;
    save_to_asm(file);
    
    file.close();
    return true;
}

void Palette::save_to_asm(std::ofstream &file)
{
    

    file<<"Palette_Start:\n";
    
    file<<".db ";

    unsigned char sms_color,sms_r,sms_g,sms_b;
    QRgb rgb_color;
    for (unsigned int i=0;i<32;i++)
    {
        if (i<16)
            rgb_color=colors_back.at(i);
        else
            rgb_color=colors_sprites.at(i-16);
        sms_b=(rgb_color&0x000000C0)>>2;
        sms_g=(rgb_color&0x0000C000)>>12;
        sms_r=(rgb_color&0x00C00000)>>22;
        sms_color=sms_r+sms_g+sms_b;
        file<<(QString("$%1").arg(sms_color,2,16,QLatin1Char('0')).toStdString());

        //std::cout<<"Convert color "<<i<<": "<<(QString("$%1").arg(rgb_color,2,16,QLatin1Char('0')).toStdString())
        //    <<" => "<<(QString("$%1").arg(sms_color,2,16,QLatin1Char('0')).toStdString())<<std::endl;

        if (i<31)
            file<<",";
    }

    file<<"\nPalette_End:\n\n";

}

bool Palette::read_from_romdata(unsigned char * romdata,long romlength,long offset)
{
    if (offset+32>=romlength)
    {
        std::cout<<"Try to read palette out of ROM!"<<std::endl;
        return false;
    }

    colors_back.clear();
    colors_sprites.clear();

    unsigned char r,g,b;
    for (int i=offset;i<offset+32;i++)
    {
        b= ((romdata[i] & 0x30)>>4)* 0x55;
        g= ((romdata[i] & 0x0C)>>2)* 0x55;
        r= ((romdata[i] & 0x03)>>0)* 0x55;
        //std::cout<<"read: 0x"<<std::hex<<(int)r<<" 0x"<<(int)g<<" 0x"<<(int)b<<std::endl;
        if (i<offset+16)
            colors_back.append( QColor(r,g,b).rgb() );
        else
            colors_sprites.append( QColor(r,g,b).rgb() );
    }
    from_offset=offset;
    return true;
}


QDomElement Palette::toNode(QDomDocument &d)
{
    QDomElement cn = d.createElement( "palette" );
    if (from_filename=="?")
        cn.setAttribute( "offset", QString("%1").arg(from_offset,0,16));
    else
    {
        cn.setAttribute( "file", from_filename );
        if (from_offset>-1)
        {
            cn.setAttribute( "in_rom_at", QString("%1").arg(from_offset,0,16) );
            cn.setAttribute( "correct_bytes_in_rom_over32", QString("%1").arg(m_correct_bytes_in_rom) );

        }
    }

    cn.setAttribute( "description", m_description );

    return cn;
}

void Palette::read_from_image(QImage *img)
{
    QVector<QRgb> colortable=img->colorTable();
    for (unsigned int i=0;i<colortable.size();i++)
    {
        //std::cout<<"Read color "<<i<<": "<<(QString("$%1").arg(colortable[i],2,16,QLatin1Char('0')).toStdString())<<std::endl;            
        if (m_sprites_palette)
        {
            colors_sprites[i]=colortable[i];
        }else{
            colors_back[i]=colortable[i];
        }
    }
}
