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
    std::cout<<"Repaint!"<<std::endl;
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
