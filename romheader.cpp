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
