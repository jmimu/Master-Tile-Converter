#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>

#include <iostream>
#include <fstream>
#include <vector>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tiles(),palette(0)
{
    ui->setupUi(this);
    QObject::connect(ui->actionOpen_Rom, SIGNAL(activated()), this, SLOT(loadROM()));
    QObject::connect(ui->actionImport_Palette, SIGNAL(activated()), this, SLOT(loadPalette()));
    QObject::connect(ui->background_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));
    QObject::connect(ui->sprite_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));

    palette=new Palette();
    ui->palettewidget->set_palette(palette);
    for (int i=0;i<12*16;i++)
        tiles.push_back(new Tile3bpp(palette));
    ui->tileswidget->set_tiles(&tiles);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


bool MainWindow::loadPalette()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose palette"), ".", tr("SMS Meka Raw Palette Files (*)"));
    if (fileName!="")
    {
        palette->read_from_file(fileName);
        std::vector<Tile*>::iterator it;
        for ( it=tiles.begin() ; it < tiles.end(); it++ )
        {
            (*it)->update_palette(palette);
        }
        ui->tileswidget->repaint();
        ui->palettewidget->repaint();
        return true;
    }
    return false;
}

void MainWindow::change_palette()
{
    palette->set_colors(ui->sprite_palette_radioButton->isChecked());
    std::vector<Tile*>::iterator it;
    for ( it=tiles.begin() ; it < tiles.end(); it++ )
    {
        (*it)->update_palette(palette);
    }
    ui->tileswidget->repaint();
    ui->palettewidget->repaint();
}

bool MainWindow::loadROM()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose ROM"), ".", tr("SMS ROM Files (*.sms)"));

    //from http://www.cplusplus.com/reference/iostream/istream/read/

    long romlength;
    unsigned char * romdata;

    std::ifstream is;
    is.open (fileName.toStdString().c_str(), std::ios::binary );

    // get length of file:
    is.seekg (0, std::ios::end);
    romlength = is.tellg();
    is.seekg (0, std::ios::beg);

    // allocate memory:
    romdata = new unsigned char [romlength];

    // read data as a block:
    is.read ((char*)romdata,romlength);
    is.close();

    std::cout<<"Read "<<romlength<<" bytes."<<std::endl;


    long offset_init=0x10341;
    long index=offset_init;
    std::vector<Tile*>::iterator it;
    for ( it=tiles.begin() ; it < tiles.end(); it++ )
    {
        index=(*it)->read(romdata,index);
    }
    delete[] romdata;

    ui->tileswidget->repaint();

    return true;
}
