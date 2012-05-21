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


#include "palettewidget.h"

#include <QPainter>

#include <iostream>

PaletteWidget::PaletteWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f), m_palette(0)
{
}


void PaletteWidget::set_palette(Palette* palette)
{
    m_palette=palette;
}



void PaletteWidget::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    QVector<QRgb>::iterator it;
    int x=0;
    for ( it=m_palette->get_back_colors().begin(); it < m_palette->get_back_colors().end(); it++ )
    {
        painter.fillRect(x*16,0,16,20,(*it));
        x++;
    }
    x=0;
    for ( it=m_palette->get_sprites_colors().begin(); it < m_palette->get_sprites_colors().end(); it++ )
    {
        painter.fillRect(x*16,20,16,20,(*it));
        x++;
    }
}
