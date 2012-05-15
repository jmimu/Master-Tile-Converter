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
