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
    painter.fillRect(0,0,8,8,Qt::DiagCrossPattern);
    if ((m_tile)&&(!m_tile->is_empty()))
        painter.drawImage(0 , 0 , m_tile->image);
}
