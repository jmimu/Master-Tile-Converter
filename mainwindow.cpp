/*MASTER TILE CONVERTER

Copyright (C) 2012  JM Muller

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QInputDialog>
#include <QMessageBox>

#include <QRegExp>

#include <iostream>
#include <sstream>
#include <fstream>

#include "dialog_about.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    palette(),real_rom(&palette),decompressed_rom(&palette),current_rom_shown(&real_rom)
{
    std::cout<<"Setup UI..."<<std::endl;
    ui->setupUi(this);
    QObject::connect(ui->actionOpen_Rom, SIGNAL(activated()), this, SLOT(loadROM()));
    QObject::connect(ui->actionSave_Rom, SIGNAL(activated()), this, SLOT(saveROM()));
    QObject::connect(ui->actionImport_Palette, SIGNAL(activated()), this, SLOT(loadPalette()));
    QObject::connect(ui->actionAbout, SIGNAL(activated()), this, SLOT(show_about()));
    QObject::connect(ui->actionImport_Compressed_Data, SIGNAL(activated()), this, SLOT(import_compressed_data()));

    QObject::connect(ui->actionCompress_Picture, SIGNAL(activated()), this, SLOT(compress_picture()));
    QObject::connect(ui->actionCompressed_and_Import_Data, SIGNAL(activated()), this, SLOT(compress_and_import()));
    QObject::connect(ui->actionDecompress_Tiles, SIGNAL(activated()), this, SLOT(decompress_tiles()));

    QObject::connect(ui->find_next_compr_pushButton, SIGNAL(pressed()), this, SLOT(decompress_tiles()));
    QObject::connect(ui->return_realrom_pushButton, SIGNAL(pressed()), this, SLOT(return_to_real_rom()));


    QObject::connect(ui->actionApply_Hack_File_fast, SIGNAL(activated()), this, SLOT(applyHackFile_fast()));
    QObject::connect(ui->actionApply_Hack_File_confirm, SIGNAL(activated()), this, SLOT(applyHackFile()));


    QObject::connect(ui->background_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));
    QObject::connect(ui->sprite_palette_radioButton, SIGNAL(clicked()), this, SLOT(change_palette()));
    QObject::connect(ui->tileswidget, SIGNAL(change_selected_tile(int)), this, SLOT(update_tiles()));//all click coordinates treatement is done in "void TilesWidget::mousePressEvent( QMouseEvent *event )"
    QObject::connect(ui->offset_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(enable_offset_button()));
    QObject::connect(ui->offset_lineEdit, SIGNAL(returnPressed()), this, SLOT(apply_offset()));
    QObject::connect(ui->Apply_offset_pushButton, SIGNAL(pressed()), this, SLOT(apply_offset()));

    QObject::connect(ui->tilesScrollBar, SIGNAL(valueChanged(int)), this, SLOT(change_offset_scrollbar(int)));


    QObject::connect(ui->mode_1bpp_radioButton, SIGNAL(clicked()), this, SLOT(change_mode()));
    QObject::connect(ui->mode_2bpp_radioButton, SIGNAL(clicked()), this, SLOT(change_mode()));
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
    QObject::connect(ui->up_160tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_up160tiles()));
    QObject::connect(ui->down_160tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_down160tiles()));


    std::cout<<"Init UI..."<<std::endl;
    ui->offset_lineEdit->setText(QString("%1").arg(real_rom.get_offset(),0,16));
    ui->offset_lineEdit->selectAll();
    ui->palettewidget->set_palette(&palette);
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    ui->zoomwidget->set_tile(ui->tileswidget->get_selected_tile());

    ui->tilesScrollBar->setSingleStep(16);//the unit is 1 tile
    ui->tilesScrollBar->setPageStep(160);

    std::cout<<"Init finished!"<<std::endl;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_about()
{
    Dialog_About dialog;
    dialog.show();
    dialog.exec();

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

//TODO: enable decompressed rom offset?
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

    if (currently_showing_decompressed_data())
    {
        return_to_real_rom();
    }

    real_rom.create_tiles(offset);
    ui->Apply_offset_pushButton->setEnabled(false);
    ui->tile_offset_label->setText(QString("Tile Offset: 0x%1").arg(real_rom.get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));

    //at first we have to disconnect the scrollbar from change_offset_scrollbar, otherwise we will have infinite loop
    ui->tilesScrollBar->disconnect();
    ui->tilesScrollBar->setMaximum(real_rom.get_romlength()/(Tile::tile_size()));//the unit for the scrollbar is 1 tile
    ui->tilesScrollBar->setValue(offset/(Tile::tile_size()));
    QObject::connect(ui->tilesScrollBar, SIGNAL(valueChanged(int)), this, SLOT(change_offset_scrollbar(int)));//re-connect

    ui->tileswidget->repaint();
    ui->zoomwidget->repaint();

    return true;
}


void MainWindow::change_offset_scrollbar(int val)
{
    //previous value of the scrollbar has to be real_rom.get_offset()/(Tile::tile_size())

    //std::cout<<"scroll "<<(float)val<<" "<<(float)(real_rom.get_offset()/(Tile::tile_size()))<<std::endl;
    long difference=(val-real_rom.get_offset()/(Tile::tile_size()))/16;//number of lines
    if (difference>0)
    {
        //move_down16tiles();
        long offset=real_rom.get_offset();
        offset+=Tile::tile_size()*difference*16;
        if (offset<0) offset=0;
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        apply_offset();

    }else{
        //move_up16tiles();
        long offset=real_rom.get_offset();
        offset+=Tile::tile_size()*difference*16;
        if (offset<0) offset=0;
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        apply_offset();
    }

    //ui->offset_lineEdit->setText(QString("%1").arg(val*(Tile::tile_size()*16),0,16));
    //apply_offset();
}

void MainWindow::enable_offset_button()
{
    ui->Apply_offset_pushButton->setEnabled(true);
}

void MainWindow::update_tiles()
{
    std::vector<Tile*>::iterator it;
    for ( it=current_rom_shown->get_tiles()->begin() ; it < current_rom_shown->get_tiles()->end(); it++ )
    {
        (*it)->update_palette(&palette);
    }
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    ui->palettewidget->set_palette(&palette);
    ui->tile_offset_label->setText(QString("Tile Offset: 0x%1").arg(current_rom_shown->get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));
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
        real_rom.loadfile(fileName.toStdString());

        return apply_offset();
    }
    return false;
}

bool MainWindow::saveROM()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ROM"),"out.sms",tr("SMS ROM (*.sms)"));
    if (fileName!="")
        return real_rom.save_ROM(fileName.toStdString());
    return false;
}

//TODO: change file name depending if decompressed data or not
bool MainWindow::export_picture()
{
    std::stringstream oss_default_bmp_name;
    oss_default_bmp_name<<"dump_0x"<<std::hex<<real_rom.get_offset()<<"_";

    unsigned int nb_bpp=4;
    if (ui->mode_1bpp_radioButton->isChecked())
        nb_bpp=1;
    else if (ui->mode_2bpp_radioButton->isChecked())
        nb_bpp=2;
    else if (ui->mode_3bpp_radioButton->isChecked())
        nb_bpp=3;
    else
        nb_bpp=4;

    oss_default_bmp_name<<nb_bpp<<"bpp";

    //if compressed
    if (current_rom_shown==&decompressed_rom)
    {
        oss_default_bmp_name<<std::dec<<"_decompressed_"<<decompressed_rom.get_tiles()->size()<<"tiles_max_"<<decompressed_rom.get_compressed_size()<<"bytes";
    }

    oss_default_bmp_name<<".BMP";
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export to BMP"),oss_default_bmp_name.str().c_str(),tr("BMP Images (*.bmp *.BMP)"));
    if (fileName!="")
        return current_rom_shown->export_BMP(fileName.toStdString(),nb_bpp,decompressed_rom.get_tiles()->size());
    return false;

}

bool MainWindow::import_picture()
{
    unsigned int nb_bpp=4;
    if (ui->mode_1bpp_radioButton->isChecked())
        nb_bpp=1;
    else if (ui->mode_2bpp_radioButton->isChecked())
        nb_bpp=2;
    else if (ui->mode_3bpp_radioButton->isChecked())
        nb_bpp=3;
    else
        nb_bpp=4;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose BMP"), ".", tr("BMP Images (*.bmp *.BMP)"));
    if (fileName!="")
    {
        if (real_rom.import_BMP(fileName.toStdString(),nb_bpp))
        {
            update_tiles();
            std::cout<<"Import OK!"<<std::endl;
            return true;
        }
        std::cout<<"Import error..."<<std::endl;
    }
    return false;

}

bool MainWindow::import_compressed_data()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose data file"), ".", tr("Tile Decoder compressed data (*.dat);;All files (*)"));
    if (fileName!="")
    {
        if (real_rom.import_rawdata(fileName.toStdString(),real_rom.get_offset()))
        {
            //update display
            if (currently_showing_decompressed_data()){
                apply_offset();
                decompress_tiles();
                update_tiles();
            }else{
                update_tiles();
            }
            return true;
        }else{
            return false;
        }
    }
    return false;
}

void MainWindow::change_mode()
{
    if (ui->mode_1bpp_radioButton->isChecked())
        Tile::number_bpp=1;
    if (ui->mode_2bpp_radioButton->isChecked())
        Tile::number_bpp=2;
    if (ui->mode_3bpp_radioButton->isChecked())
        Tile::number_bpp=3;
    if (ui->mode_4bpp_radioButton->isChecked())
        Tile::number_bpp=4;

    apply_offset();
}


bool MainWindow::applyHackFile(bool confirm)
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose hack file"), ".", tr("text File (*.txt);;All files (*)"));
    if (fileName!="")
    {
        std::ifstream is;
        is.open (fileName.toStdString().c_str());
        if (is.fail())
        {
            std::cerr<<"Can not open file: "<<fileName.toStdString()<<std::endl;
            return false;
        }
        std::string line;
        while(getline(is,line))
        {
            QRegExp rx("^ *[^;]");
            if (rx.indexIn(line.c_str())!=-1)
            {
                QRegExp rx2("^ *([0-9A-Fa-f]+) *\\: *([0-9A-Fa-f ]+) *\\*(.*)$");
                if (rx2.indexIn(line.c_str()) != -1) {
                    //std::cout<<"Got: "<<rx2.cap(1).toStdString()<<" | "<<rx2.cap(2).toStdString()<<" | "<<rx2.cap(3).toStdString()<<std::endl;
                    bool ok;
                    unsigned long address=rx2.cap(1).toLong(&ok,16);
                    if (!ok){
                        std::cerr<<"Problem reading hex address: "<<rx2.cap(1).toStdString()<<std::endl;
                        break;
                    }
                    std::vector<unsigned char> data;
                    QStringList data_str_list = rx2.cap(2).split(" ",QString::SkipEmptyParts);
                    for (int i = 0; i < data_str_list.size(); ++i)
                    {

                        //std::cout<<"Try : "<<data_str_list.at(i).toStdString()<<std::endl;
                        data.push_back(data_str_list.at(i).toInt(&ok,16));
                        if (!ok){
                            std::cerr<<"Problem reading hex data: "<<data_str_list.at(i).toStdString()<<std::endl;
                            break;
                        }
                    }
                    std::string comment=rx2.cap(3).toStdString();

                    bool do_it=true;
                    if (confirm)
                    {
                        //show message...
                        //do_it=...
                        QMessageBox msgBox;
                        msgBox.setText(QString("Apply modification \"")+rx2.cap(3)+"\"?");
                        msgBox.setInformativeText(QString("At ")+rx2.cap(1)+", write:\n"+rx2.cap(2));
                        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                        msgBox.setDefaultButton(QMessageBox::Yes);
                        msgBox.setIcon(QMessageBox::Question);
                        do_it=(msgBox.exec()==QMessageBox::Yes);
                    }
                    if (do_it)
                        if (!real_rom.set_romdata(address,&data))
                        {
                            std::cerr<<"Modification out of rom: "<<line<<std::endl;
                        }
                }
            }
        }

        is.close();

        return true;
    }
    return false;
}


bool MainWindow::compress_picture()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose BMP file"), ".", tr("BMP File (*.bmp)"));
    if (fileName!="")
    {
        Rom rom_tmp(&palette);
        if (rom_tmp.import_BMP(fileName.toStdString(),4))
        {
            bool ok;
            unsigned long nbr_total_tiles=rom_tmp.get_romlength()/4/8;
            int nb_tiles = QInputDialog::getInt(this, tr("How many tiles to compress?"),
                                         tr("Number of tiles for file\n\"")+fileName+"\"?", nbr_total_tiles, 0, nbr_total_tiles, 1, &ok);
            unsigned long nb_bytes=rom_tmp.compress_tiles(nb_tiles);

            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this, "Compression done.", QString("The %1 tiles have been compressed to \"compr_tmp.dat\" in MTC folder. Its size is %2 bytes").arg(nb_tiles).arg(nb_bytes));
            //if (reply == QMessageBox::Ok)

            return true;
        }
    }
    return false;
}

//creates the file compr_tmp.dat in MTC folder
bool MainWindow::compress_and_import()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose BMP file"), ".", tr("BMP File (*.bmp)"));
    if (fileName!="")
    {
        Rom rom_tmp(&palette);
        if (rom_tmp.import_BMP(fileName.toStdString(),4))
        {
            bool ok;
            unsigned long nbr_total_tiles=rom_tmp.get_romlength()/4/8;
            int nb_tiles = QInputDialog::getInt(this, tr("How many tiles to compress?"),
                                         tr("Number of tiles for file\n\"")+fileName+"\"?", nbr_total_tiles, 0, nbr_total_tiles, 1, &ok);
            unsigned long nb_bytes=rom_tmp.compress_tiles(nb_tiles);

            QMessageBox::StandardButton reply;
            reply = QMessageBox::information(this, "Compression done.", QString("The %1 tiles have been compressed to \"compr_tmp.dat\". Its size is %2 bytes").arg(nb_tiles).arg(nb_bytes));

            //part from import_rawdata
            if (real_rom.import_rawdata("compr_tmp.dat",real_rom.get_offset()))
            {
                //update display
                if (currently_showing_decompressed_data()){
                    apply_offset();
                    decompress_tiles();
                    update_tiles();
                }else{
                    update_tiles();
                }
                return true;
            }else{
                return false;
            }

        }
    }
    return false;
}


bool MainWindow::decompress_tiles()
{
    //set 4bpp mode
    if (Tile::number_bpp!=4)
    {
        ui->mode_4bpp_radioButton->setChecked(true);
        change_mode();
    }

    ui->Apply_offset_pushButton->setEnabled(false);

    bool ok;
    unsigned long offset=0;
    offset=ui->offset_lineEdit->text().toULong(&ok,16);

    if (currently_showing_decompressed_data())
    {
        offset++;//if already showing decompressed tiles, search for next
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        ui->Apply_offset_pushButton->setEnabled(false);//do not let apply offset button enabled while offset changes
        ui->tile_offset_label->repaint();
        real_rom.set_offset(offset);
    }


    while (!decompressed_rom.test_decompress_tiles(&real_rom,offset))
    {
        //move_down1Byte();
        offset++;
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        ui->Apply_offset_pushButton->setEnabled(false);//do not let apply offset button enabled while offset changes
        ui->tile_offset_label->repaint();
        real_rom.set_offset(offset);
    }
    if (offset>=real_rom.get_romlength())
    {
        std::cout<<"Reached end of ROM. No more compressed data."<<std::endl;
        offset=real_rom.get_romlength();
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        ui->Apply_offset_pushButton->setEnabled(false);//do not let apply offset button enabled while offset changes
        ui->tile_offset_label->repaint();
        real_rom.set_offset(offset);
        return_to_real_rom();
        return false;
    }
    decompressed_rom.decompress_tiles(&real_rom,offset);
    decompressed_rom.create_tiles(0);
    update_tiles();
    show_decompressed_data();


    return (true);
}

void MainWindow::show_decompressed_data()
{
    ui->tilesScrollBar->setEnabled(false);
    ui->label->setText("Compressed Tiles");
    current_rom_shown=&decompressed_rom;
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    ui->tileswidget->repaint();

    ui->down_160tiles_pushButton->setEnabled(false);
    ui->down_16tiles_pushButton->setEnabled(false);
    ui->down_1Byte_pushButton->setEnabled(false);
    ui->down_1row_pushButton->setEnabled(false);
    ui->down_1tile_pushButton->setEnabled(false);
    ui->up_160tiles_pushButton->setEnabled(false);
    ui->up_16tiles_pushButton->setEnabled(false);
    ui->up_1Byte_pushButton->setEnabled(false);
    ui->up_1row_pushButton->setEnabled(false);
    ui->up_1tile_pushButton->setEnabled(false);
    ui->return_realrom_pushButton->setEnabled(true);
}


void MainWindow::return_to_real_rom()
{
    ui->tilesScrollBar->setEnabled(true);
    ui->label->setText("Tiles");
    current_rom_shown=&real_rom;
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    apply_offset();
    change_mode();
    ui->tileswidget->repaint();

    ui->down_160tiles_pushButton->setEnabled(true);
    ui->down_16tiles_pushButton->setEnabled(true);
    ui->down_1Byte_pushButton->setEnabled(true);
    ui->down_1row_pushButton->setEnabled(true);
    ui->down_1tile_pushButton->setEnabled(true);
    ui->up_160tiles_pushButton->setEnabled(true);
    ui->up_16tiles_pushButton->setEnabled(true);
    ui->up_1Byte_pushButton->setEnabled(true);
    ui->up_1row_pushButton->setEnabled(true);
    ui->up_1tile_pushButton->setEnabled(true);
    ui->return_realrom_pushButton->setEnabled(false);
}


//TODO: able to move in decompressed rom?
void MainWindow::move_up1Byte()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=1;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1Byte()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset+=1;
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up1row()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=Tile::row_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1row()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset+=Tile::row_size();
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up1tile()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=Tile::tile_size();
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down1tile()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset+=Tile::tile_size();
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up16tiles()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=Tile::tile_size()*16;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down16tiles()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset+=Tile::tile_size()*16;
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up160tiles()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=Tile::tile_size()*160;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down160tiles()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset+=Tile::tile_size()*160;
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}
