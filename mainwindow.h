#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

#include "tile3bpp.h"
#include "palette.h"
#include "rom.h"

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
    Palette palette;
    Rom rom;

public slots:
    bool loadROM();
    bool loadPalette();
    void change_palette();
    void update_tiles();
    void enable_offset_button();
    bool apply_offset();
};

#endif // MAINWINDOW_H
