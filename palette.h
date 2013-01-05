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
#include <QString>
#include <QtXml/QDomDocument>

class Palette
{
public:
    Palette(QString description);
    bool read_from_file(QString fileName);
    bool read_from_romdata(unsigned char * data,long offset);
    QVector<QRgb> &get_colors() { if (m_sprites_palette) return colors_sprites; else return colors_back;};
    QVector<QRgb> &get_back_colors() { return colors_back; };
    QVector<QRgb> &get_sprites_colors() { return colors_sprites; };
    void set_colors(bool sprites_palette) { m_sprites_palette=sprites_palette; };
    QString get_description(){return m_description;};
    QDomElement toNode(QDomDocument &d);
protected:
    bool m_sprites_palette;//which palette?
    QString m_description;
    QVector<QRgb> colors_back;//format for QImage
    QVector<QRgb> colors_sprites;//format for QImage

    //origin of the palette
    QString from_filename;
    long from_offset;
};

#endif // PALETTE_H
