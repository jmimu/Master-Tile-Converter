Master-Tile-Converter
=====================

Extract and import 3bpp and 4bpp tiles from Sega Master System (SMS) roms.


Usage :
 - load ROM
 - load palette (binary dump form meka)
 - find the perfect position in the rom where you want to work
 - select the good format (3bpp or 4 bpp)
 - select the correct part of the palette
 - export !

 - edit BMP in an external editor (don't modify the palette! don't change tiles with no meaning!)
(you can change the size of the picture)
* make your own graphics (respect size and palette)
* rearrange tiles to have the same layout as your dump (you may have to be pixel-precise!)
* add transparent for the tiles you don't want to change
* past it in the dump image in GIMP (it respects the palette)
* save it as you new image

 - back in MTC, if you are in the correct position, import your BMP.
 - Save the resulting rom.
Done !








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
