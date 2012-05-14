#include "tileswidget.h"

#include <QPainter>

#include <iostream>

TilesWidget::TilesWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f), m_tiles(0)
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
    painter.fillRect(0,0,16*8+2,12*8+2,Qt::darkCyan);
    //painter.setPen(Qt::darkCyan);
    //painter.setBackgroundMode(Qt::OpaqueMode);
    //painter.setBackground(QBrush(Qt::darkCyan,Qt::SolidPattern));
    if (m_tiles)
    {
        long x=0;
        long y=0;
        std::vector<Tile*>::iterator it;
        for ( it=m_tiles->begin() ; it < m_tiles->end(); it++ )
        {
            painter.drawImage(1+x*8 , 1+y*8 , (*it)->image);
            x++;
            if (x>=16)
            {
                x=0;
                y++;
            }
        }
    }
}
