#ifndef TILESWIDGET_H
#define TILESWIDGET_H

#include <QWidget>

#include <vector>

#include "tile.h"

class TilesWidget : public QWidget
{
public:
    TilesWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    void set_tiles(std::vector<Tile*> * tiles);
protected:
    void paintEvent(QPaintEvent*);
    std::vector<Tile*> * m_tiles;
};

#endif // TILESWIDGET_H
