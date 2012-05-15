#ifndef TILESWIDGET_H
#define TILESWIDGET_H

#include <QWidget>

#include <vector>

#include "tile.h"

class TilesWidget : public QWidget
{
    Q_OBJECT
public:
    TilesWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    void set_tiles(std::vector<Tile*> * tiles);
    int nb_tiles_width;
    Tile* get_selected_tile(){ if (m_tiles) return m_tiles->at(selected_tile);else return 0;};
protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent( QMouseEvent* event);
    std::vector<Tile*> * m_tiles;
    int selected_tile;//number of the selected tile
signals:
    void change_selected_tile(int);
};

#endif // TILESWIDGET_H
