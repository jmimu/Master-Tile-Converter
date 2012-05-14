#ifndef PALETTE_H
#define PALETTE_H

#include <QColor>
#include <QRgb>
#include <QVector>


class Palette
{
public:
    Palette();
    bool read_from_file(QString fileName);
    QVector<QRgb> &get_colors() { if (m_sprites_palette) return colors_sprites; else return colors_back;};
    QVector<QRgb> &get_back_colors() { return colors_back; };
    QVector<QRgb> &get_sprites_colors() { return colors_sprites; };
    void set_colors(bool sprites_palette) { m_sprites_palette=sprites_palette; }
protected:
    bool m_sprites_palette;//which palette?
    QVector<QRgb> colors_back;//format for QImage
    QVector<QRgb> colors_sprites;//format for QImage
};

#endif // PALETTE_H
