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


#ifndef TILESWIDGET_H
#define TILESWIDGET_H

#include <QWidget>

#include <vector>

#include "tile.h"

#define NB_TILES_LINES 14

class TilesWidget : public QWidget
{
    Q_OBJECT
public:
    TilesWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    void set_tiles(std::vector<Tile*> * tiles);
    int nb_tiles_width;
    Tile* get_selected_tile(){ if (m_tiles) return m_tiles->at(selected_tile);else return 0;}
    int get_selection_number(){return selected_tile;}
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent( QMouseEvent* event);
    std::vector<Tile*> * m_tiles;
    int selected_tile;//number of the selected tile
signals:
    void change_selected_tile(int);
};

#endif // TILESWIDGET_H
