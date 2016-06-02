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

#ifndef SYSTEM_H
#define SYSTEM_H


class Rom;

//Interface for each supported console
class System
{
public:
    System();
    virtual ~System();
    void setROM(Rom *rom);
    virtual bool check_ROM()=0;
    virtual unsigned short read_checksum()=0;//get the checksum written in header
    virtual void read_region_and_size()=0;
    virtual unsigned short compute_checksum()=0;
    virtual void fix_checksum()=0;
protected:
    Rom * m_rom;
    unsigned short m_computed_checksum;
    unsigned short m_read_checksum;
    unsigned char m_region;
    unsigned char m_rom_size;
};

#endif // SYSTEM_H
