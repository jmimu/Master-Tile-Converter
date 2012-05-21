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


#ifndef PALETTEWIDGET_H
#define PALETTEWIDGET_H

#include <QWidget>

#include "palette.h"

class PaletteWidget : public QWidget
{
public:
    PaletteWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    void set_palette(Palette* palette);
protected:
    void paintEvent(QPaintEvent*);
    Palette *m_palette;
};

#endif // PALETTEWIDGET_H
