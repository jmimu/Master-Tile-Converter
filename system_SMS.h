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


#ifndef SYSTEM_SMS_H
#define SYSTEM_SMS_H

#include "system.h"

//see http://www.smspower.org/Development/ROMHeader

//Some code taken from:
//---------------------------------------------------------------------------
// SMS Check (c) Omar Cornut (Bock) 2000-2006
// Sega 8-bit ROM tool, showing checksum and various informations
//---------------------------------------------------------------------------

//$1ff0, $3ff0 or $7ff0
#define HEADER_ADDRESS 0x7ff0
#define CHECKSUM_ADDRESS 0x7ffa
#define REGION_AND_SIZE_ADDRESS 0x7fff
#define SMS_HEADER_SIZE 0x10

/* regions (the high 4 bits at REGION_AND_SIZE_ADDRESS)
$3	SMS Japan
$4	SMS Export
$5	GG Japan
$6	GG Export
$7	GG International

 size (the low 4 bits at REGION_AND_SIZE_ADDRESS)
$a	8KB	Unused
$b	16KB	Unused
$c	32KB
$d	48KB	Unused, buggy
$e	64KB	Rarely used
$f	128KB
$0	256KB
$1	512KB	Rarely used
$2	1MB	Unused, buggy
*/



class System_SMS:public System
{
public:
    System_SMS();
    virtual bool check_ROM();
    virtual unsigned short read_checksum();//get the checksum written in header
    virtual void read_region_and_size();
    virtual unsigned short compute_checksum();
    virtual void fix_checksum();
/*protected:
    Rom * m_rom;
    unsigned short m_computed_checksum;
    unsigned short m_read_checksum;
    unsigned char m_region;
    unsigned char m_rom_size;*/

};

#endif // SYSTEM_SMS_H
