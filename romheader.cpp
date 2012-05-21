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


#include "romheader.h"

#include <iostream>

ROMHeader::ROMHeader(Rom *rom):m_rom(rom),m_computed_checksum(0),m_read_checksum(0),m_region(0),m_rom_size(0)
{
}

bool ROMHeader::check_TMR_SEGA()
{
    char * signature=(char*)"TMR SEGA";
    for (int i=0;i<8;i++)
        if (m_rom->get_romdata()[HEADER_ADDRESS+i]!=signature[i])
        {
            std::cout<<"\"TMR SEGA\" test failed."<<std::endl;
            return false;
        }
    std::cout<<"\"TMR SEGA\" test passed!"<<std::endl;
    return true;
}


unsigned short ROMHeader::read_checksum()
{
    unsigned short checksum=0;
    checksum=m_rom->get_romdata()[CHECKSUM_ADDRESS+1];
    checksum=checksum<<8;
    checksum+=m_rom->get_romdata()[CHECKSUM_ADDRESS];
    m_read_checksum=checksum;
    return checksum;
}

void ROMHeader::read_region_and_size()
{
    m_region=m_rom->get_romdata()[REGION_AND_SIZE_ADDRESS]>>4;
    m_rom_size=m_rom->get_romdata()[REGION_AND_SIZE_ADDRESS]%0x10;
    std::cout<<"Region code: "<<std::hex<<(int)m_region<<"; size code: "<<std::hex<<(int)m_rom_size<<std::endl;
}


//Code taken from:
//---------------------------------------------------------------------------
// SMS Check (c) Omar Cornut (Bock) 2000-2006
// Sega 8-bit ROM tool, showing checksum and various informations
//---------------------------------------------------------------------------
unsigned short ROMHeader::compute_checksum()
{
    read_region_and_size();

    long checksum_len;
    switch (m_rom_size)
    {
        case 0x0A: checksum_len = 0x02000;   break;
        case 0x0B: checksum_len = 0x04000;   break;
        case 0x0C: checksum_len = 0x08000;   break;
        case 0x0D: checksum_len = 0x0C000;   break;
        case 0x0E: checksum_len = 0x10000;   break;
        case 0x0F: checksum_len = 0x20000;   break;
        case 0x00: checksum_len = 0x40000;   break;
        default:   checksum_len = m_rom->get_romlength();  break;
    }

    m_computed_checksum=0;
    if (m_rom->get_romlength() < checksum_len)
    {
        std::cout<<"File too short for checksum!\n";
    }
    else
    {
        // Compute checksum
        m_computed_checksum = 0;
        for (long i = 0; i < checksum_len; i++)
            m_computed_checksum += m_rom->get_romdata()[i];
        // Remove header from checksum
        if (checksum_len >= 0x8000)
            for (long i = 0x8000 - SMS_HEADER_SIZE; i < 0x8000; i++)
                m_computed_checksum -= m_rom->get_romdata()[i];
        // Print out result
        std::cout<<"Computed checksum: "<<std::hex<<m_computed_checksum<<std::endl;
    }

    return m_computed_checksum;
}


void ROMHeader::fix_checksum()
{
    compute_checksum();

    std::cout<<"Fix checksum:\nreplace 0x"<<std::hex<<(int)m_rom->get_romdata()[CHECKSUM_ADDRESS]<<" by 0x"<<std::hex<<(int)(unsigned char)(m_computed_checksum%0x100)<<std::endl;
    std::cout<<"replace 0x"<<std::hex<<(int)m_rom->get_romdata()[CHECKSUM_ADDRESS+1]<<" by 0x"<<std::hex<<(int)(unsigned char)(m_computed_checksum>>8)<<std::endl;
    m_rom->get_romdata()[CHECKSUM_ADDRESS]  =(unsigned char)(m_computed_checksum%0x100);
    m_rom->get_romdata()[CHECKSUM_ADDRESS+1]=(unsigned char)(m_computed_checksum>>8);
}

