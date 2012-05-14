#ifndef PALETTEWIDGET_H
#define PALETTEWIDGET_H

#include <QWidget>

#include "palette.h"

class PaletteWidget : public QWidget
{
public:
    PaletteWidget(QWidget * parent = 0, Qt::WindowFlags f = 0 );
    void set_palette(Palette* palette);
protected:
    void paintEvent(QPaintEvent*);
    Palette *m_palette;
};

#endif // PALETTEWIDGET_H
