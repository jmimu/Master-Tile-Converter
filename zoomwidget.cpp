#include "zoomwidget.h"

#include <QPainter>
#include <iostream>

ZoomWidget::ZoomWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f), m_tile(0)
{

}

void ZoomWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.scale(16,16);
    painter.fillRect(0,0,8,8,Qt::darkCyan);
    if (m_tile)
        painter.drawImage(0 , 0 , m_tile->image);
}
