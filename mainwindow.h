#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

#include "tile3bpp.h"
#include "palette.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    std::vector<Tile*> tiles;
    Palette *palette;

public slots:
    bool loadROM();
    bool loadPalette();
    void change_palette();
    void update_tiles();
};

#endif // MAINWINDOW_H
