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
#include "system_unknown.h"

#include <iostream>
#include "rom.h"

System_Unknown::System_Unknown():System()
{
}


bool System_Unknown::check_ROM()
{
    return true;
}


unsigned short System_Unknown::read_checksum()
{
    unsigned short checksum=0;
    return checksum;
}

void System_Unknown::read_region_and_size()
{
    m_region=0;
    m_rom_size=0;
    std::cout<<"Region code: "<<std::hex<<(int)m_region<<"; size code: "<<std::hex<<(int)m_rom_size<<std::endl;
}


unsigned short System_Unknown::compute_checksum()
{
    m_computed_checksum=0;
    return m_computed_checksum;
}


void System_Unknown::fix_checksum()
{

}

