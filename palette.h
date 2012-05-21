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


#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QRgb>
#include <QVector>


class Palette
{
public:
    Palette();
    bool read_from_file(QString fileName);
    QVector<QRgb> &get_colors() { if (m_sprites_palette) return colors_sprites; else return colors_back;};
    QVector<QRgb> &get_back_colors() { return colors_back; };
    QVector<QRgb> &get_sprites_colors() { return colors_sprites; };
    void set_colors(bool sprites_palette) { m_sprites_palette=sprites_palette; }
protected:
    bool m_sprites_palette;//which palette?
    QVector<QRgb> colors_back;//format for QImage
    QVector<QRgb> colors_sprites;//format for QImage
};

#endif // PALETTE_H
