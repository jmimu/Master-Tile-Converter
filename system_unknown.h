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

#ifndef SYSTEM_UNKNOWN_H
#define SYSTEM_UNKNOWN_H

#include "system.h"

class System_Unknown : public System
{
public:
    System_Unknown();
    virtual bool check_ROM();
    virtual unsigned short read_checksum();//get the checksum written in header
    virtual void read_region_and_size();
    virtual unsigned short compute_checksum();
    virtual void fix_checksum();
};

#endif // SYSTEM_UNKNOWN_H
