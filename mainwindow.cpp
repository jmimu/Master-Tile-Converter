#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>

#include <iostream>
#include <sstream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    palette(),rom(&palette)
{
    std::cout<<"Setup UI..."<<std::endl;
    ui->setupUi(this);
    QObject::connect(ui->actionOpen_Rom, SIGNAL(activated()), this, SLOT(loadROM()));
    QObject::connect(ui->actionSave_Rom, SIGNAL(activated()), this, SLOT(saveROM()));
    QObject::connect(ui->actionImport_Palette, SIGNAL(activated()), this, SLOT(loadPalette()));
    QObject::connect(ui->background_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));
    QObject::connect(ui->sprite_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));
    QObject::connect(ui->tileswidget, SIGNAL(change_selected_tile(int)), this, SLOT(update_tiles()));//all click coordinates treatement is done in "void TilesWidget::mousePressEvent( QMouseEvent *event )"
    QObject::connect(ui->offset_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(enable_offset_button()));
    QObject::connect(ui->offset_lineEdit, SIGNAL(returnPressed()), this, SLOT(apply_offset()));
    QObject::connect(ui->Apply_offset_pushButton, SIGNAL(pressed()), this, SLOT(apply_offset()));

    QObject::connect(ui->mode_3bpp_radioButton, SIGNAL(clicked()), this, SLOT(change_mode()));
    QObject::connect(ui->mode_4bpp_radioButton, SIGNAL(clicked()), this, SLOT(change_mode()));

    QObject::connect(ui->actionExport_Picture, SIGNAL(activated()), this, SLOT(export_picture()));
    QObject::connect(ui->actionImport_Picture, SIGNAL(activated()), this, SLOT(import_picture()));


    QObject::connect(ui->up_1Byte_pushButton, SIGNAL(pressed()), this, SLOT(move_up1Byte()));
    QObject::connect(ui->down_1Byte_pushButton, SIGNAL(pressed()), this, SLOT(move_down1Byte()));
    QObject::connect(ui->up_1row_pushButton, SIGNAL(pressed()), this, SLOT(move_up1row()));
    QObject::connect(ui->down_1row_pushButton, SIGNAL(pressed()), this, SLOT(move_down1row()));
    QObject::connect(ui->up_1tile_pushButton, SIGNAL(pressed()), this, SLOT(move_up1tile()));
    QObject::connect(ui->down_1tile_pushButton, SIGNAL(pressed()), this, SLOT(move_down1tile()));
    QObject::connect(ui->up_16tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_up16tiles()));
    QObject::connect(ui->down_16tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_down16tiles()));


    std::cout<<"Init UI..."<<std::endl;
    ui->offset_lineEdit->setText(QString("%1").arg(rom.get_offset(),0,16));
    ui->palettewidget->set_palette(&palette);
    ui->tileswidget->set_tiles(rom.get_tiles());
    ui->zoomwidget->set_tile(ui->tileswidget->get_selected_tile());

    std::cout<<"Init finished!"<<std::endl;
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

bool MainWindow::apply_offset()
{
    bool ok;
    unsigned long offset=0;
    offset=ui->offset_lineEdit->text().toULong(&ok,16);
    if (!ok)
    {
        std::cerr<<"Error in offset!"<<std::endl;
        return false;
    }
    rom.create_tiles(offset);
    ui->Apply_offset_pushButton->setEnabled(false);
    ui->tile_offset_label->setText(QString("Tile Offset: 0x%1").arg(rom.get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));
    ui->tileswidget->repaint();
    ui->zoomwidget->repaint();

    return true;
}

void MainWindow::enable_offset_button()
{
    ui->Apply_offset_pushButton->setEnabled(true);
}

void MainWindow::update_tiles()
{
    std::vector<Tile*>::iterator it;
    for ( it=rom.get_tiles()->begin() ; it < rom.get_tiles()->end(); it++ )
    {
        (*it)->update_palette(&palette);
    }
    ui->palettewidget->set_palette(&palette);
    ui->tile_offset_label->setText(QString("Tile Offset: 0x%1").arg(rom.get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));
    ui->zoomwidget->set_tile(ui->tileswidget->get_selected_tile());
    ui->tileswidget->repaint();
    ui->palettewidget->repaint();
    ui->zoomwidget->repaint();
}

bool MainWindow::loadPalette()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose palette"), ".", tr("SMS Meka Raw Palette Files (*)"));
    if (fileName!="")
    {
        palette.read_from_file(fileName);
        update_tiles();
        return true;
    }
    return false;
}

void MainWindow::change_palette()
{
    palette.set_colors(ui->sprite_palette_radioButton->isChecked());
    update_tiles();
}

bool MainWindow::loadROM()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose ROM"), ".", tr("SMS ROM Files (*.sms)"));
    if (fileName!="")
    {
        rom.loadfile(fileName.toStdString());

        return apply_offset();
    }
    return false;
}

bool MainWindow::saveROM()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ROM"),"out.sms",tr("SMS ROM (*.sms)"));
    if (fileName!="")
        return rom.save_ROM(fileName.toStdString());
    return false;
}

bool MainWindow::export_picture()
{
    std::stringstream oss_default_bmp_name;
    oss_default_bmp_name<<"dump_0x"<<std::hex<<rom.get_offset()<<"_";
    if (ui->mode_3bpp_radioButton->isChecked())
        oss_default_bmp_name<<"3bpp";
    else
        oss_default_bmp_name<<"4bpp";
    oss_default_bmp_name<<".BMP";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export to BMP"),oss_default_bmp_name.str().c_str(),tr("BMP Images (*.bmp *.BMP)"));
    if (fileName!="")
        return rom.export_BMP(fileName.toStdString(),ui->mode_3bpp_radioButton->isChecked());
    return false;

}

bool MainWindow::import_picture()
{

    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose BMP"), ".", tr("BMP Images (*.bmp *.BMP)"));
    if (fileName!="")
    {
        if (rom.import_BMP(fileName.toStdString(),ui->mode_3bpp_radioButton->isChecked()))
        {
            update_tiles();
            std::cout<<"Import OK!"<<std::endl;
            return true;
        }
        std::cout<<"Import error..."<<std::endl;
    }
    return false;

}


void MainWindow::change_mode()
{
    Tile::is3bpp=ui->mode_3bpp_radioButton->isChecked();
    apply_offset();
}

void MainWindow::move_up1Byte()
{
    long offset=rom.get_offset();
    offset-=1;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1Byte()
{
    long offset=rom.get_offset();
    offset+=1;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up1row()
{
    long offset=rom.get_offset();
    offset-=Tile::row_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1row()
{
    long offset=rom.get_offset();
    offset+=Tile::row_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up1tile()
{
    long offset=rom.get_offset();
    offset-=Tile::tile_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1tile()
{
    long offset=rom.get_offset();
    offset+=Tile::tile_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up16tiles()
{
    long offset=rom.get_offset();
    offset-=Tile::tile_size()*16;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down16tiles()
{
    long offset=rom.get_offset();
    offset+=Tile::tile_size()*16;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}
