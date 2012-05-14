#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QRgb>
#include <QVector>


class Palette
{
public:
    Palette();
    bool read_from_file(QString fileName,bool sprites_palette=true);

    QVector<QRgb> colors;//format for QImage
};

#endif // PALETTE_H
