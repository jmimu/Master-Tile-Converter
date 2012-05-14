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
    int x=0;
    QVector<QRgb>::iterator it;
    for ( it=m_palette->colors.begin(); it < m_palette->colors.end(); it++ )
    {
        painter.fillRect(x*16,0,16,32,(*it));
        x++;
    }
}
