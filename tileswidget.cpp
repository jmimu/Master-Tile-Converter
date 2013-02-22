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


#include "tileswidget.h"

#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <iostream>

TilesWidget::TilesWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f),nb_tiles_width(16), m_tiles(0),selected_tile(0)
{
}


void TilesWidget::set_tiles(std::vector<Tile*> * tiles)
{
    m_tiles=tiles;
}

void TilesWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.scale(4,4);
    painter.fillRect(0,0,16*8,12*8,Qt::darkCyan);
    painter.fillRect(0,0,16*8,12*8,Qt::DiagCrossPattern);
    if (m_tiles)
    {
        long x=0;
        long y=0;
        std::vector<Tile*>::iterator it;
        for ( it=m_tiles->begin() ; it < m_tiles->end(); it++ )
        {
            if (!(*it)->is_empty())
                painter.drawImage(x*8 , y*8 , (*it)->image);
            x++;
            if (x>=nb_tiles_width)
            {
                x=0;
                y++;
            }
        }
        //draw selection
        y=selected_tile/nb_tiles_width;
        x=selected_tile%nb_tiles_width;
        QPen pen;
        pen.setStyle(Qt::DotLine);
        pen.setWidth(2);
        pen.setBrush(Qt::green);
        painter.setPen(pen);
        painter.drawRect(x*8,y*8,8,8);
        pen.setStyle(Qt::DotLine);
        pen.setWidth(1);
        pen.setBrush(Qt::blue);
        painter.setPen(pen);
        painter.drawRect(x*8,y*8,8,8);
    }
}


void TilesWidget::mousePressEvent( QMouseEvent *event )
{
    if (event->button()==Qt::LeftButton)
    {
        int x=(event->x()/(4*8));
        int y=(event->y()/(4*8));
        int new_selected_tile=y*nb_tiles_width+x;
        if (new_selected_tile<m_tiles->size())
        {
            selected_tile=new_selected_tile;
            emit(change_selected_tile(selected_tile));
        }
    }
    //repaint();
}
