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
    int nb_tiles_width;
    Tile* get_selected_tile(){ if (m_tiles) return m_tiles->at(selected_tile);else return 0;};
protected:
    void paintEvent(QPaintEvent*);
    std::vector<Tile*> * m_tiles;
    int selected_tile;//number of the selected tile

};

#endif // TILESWIDGET_H
