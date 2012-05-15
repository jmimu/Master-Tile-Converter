#include "tileswidget.h"

#include <QPainter>
#include <QPen>

#include <iostream>

TilesWidget::TilesWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f),nb_tiles_width(16), m_tiles(0),selected_tile(0)
{
}


void TilesWidget::set_tiles(std::vector<Tile*> * tiles)
{
    m_tiles=tiles;
}

void TilesWidget::paintEvent(QPaintEvent*) {
    std::cout<<"Repaint!"<<std::endl;




    QPainter painter(this);
    painter.scale(4,4);
    painter.fillRect(0,0,16*8,12*8,Qt::darkCyan);
    if (m_tiles)
    {
        long x=0;
        long y=0;
        std::vector<Tile*>::iterator it;
        for ( it=m_tiles->begin() ; it < m_tiles->end(); it++ )
        {
            painter.drawImage(x*8 , y*8 , (*it)->image);
            x++;
            if (x>=nb_tiles_width)
            {
                x=0;
                y++;
            }
        }
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
