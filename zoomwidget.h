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


#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>

#include "tile.h"

class ZoomWidget : public QWidget
{
public:
    ZoomWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);
    void set_tile(Tile* tile){m_tile=tile;};
protected:
    void paintEvent(QPaintEvent*);
    Tile* m_tile;
};

#endif // ZOOMWIDGET_H
