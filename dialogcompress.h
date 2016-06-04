#ifndef DIALOGCOMPRESS_H
#define DIALOGCOMPRESS_H

#include <QDialog>
#include "rom.h"
#include "mtcproject.h"

namespace Ui {
class DialogCompress;
}

class DialogCompress : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCompress(Rom *_Rom,MTCproject * _Project, int size_decompressed,QWidget *parent = 0);
    ~DialogCompress();
public slots:
    bool chooseBMP();
    bool chooseOutput();
    bool compress();
    void updateCompressButton();
private:
    Ui::DialogCompress *ui;
    Rom * mRom;
    MTCproject * mProject;
    Rom mRomTmp;
};

#endif // DIALOGCOMPRESS_H
