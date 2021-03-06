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
#include "dialogcompress.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),m_project(new MTCproject),
    real_rom(m_project->getPalette()),decompressed_rom(m_project->getPalette()),current_rom_shown(&real_rom)
{
    std::cout<<"Setup UI..."<<std::endl;
    ui->setupUi(this);

    QObject::connect(ui->actionOpen_Rom, SIGNAL(triggered()), this, SLOT(loadROM()));
    QObject::connect(ui->actionSave_Rom, SIGNAL(triggered()), this, SLOT(saveROM()));
    QObject::connect(ui->actionCreate_IPS_patch, SIGNAL(triggered()), this, SLOT(createIPS()));
    QObject::connect(ui->actionImport_Palette, SIGNAL(triggered()), this, SLOT(loadPaletteFile()));
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(show_about()));
    QObject::connect(ui->actionImport_Compressed_Data, SIGNAL(triggered()), this, SLOT(import_compressed_data()));

    QObject::connect(ui->actionCompress_Picture, SIGNAL(triggered()), this, SLOT(compress_picture()));
    QObject::connect(ui->actionDecompress_Tiles, SIGNAL(triggered()), this, SLOT(decompress_tiles()));

    QObject::connect(ui->find_next_compr_pushButton, SIGNAL(pressed()), this, SLOT(decompress_tiles()));
    QObject::connect(ui->return_realrom_pushButton, SIGNAL(pressed()), this, SLOT(return_to_real_rom()));


    QObject::connect(ui->actionApply_Hack_File_fast, SIGNAL(triggered()), this, SLOT(applyHackFile_fast()));
    QObject::connect(ui->actionApply_Hack_File_confirm, SIGNAL(triggered()), this, SLOT(applyHackFile()));


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

    QObject::connect(ui->actionExport_Picture, SIGNAL(triggered()), this, SLOT(export_picture()));
    QObject::connect(ui->actionImport_Picture, SIGNAL(triggered()), this, SLOT(import_picture()));
    QObject::connect(ui->actionSave_Project, SIGNAL(triggered()), this, SLOT(saveMTCproject()));
    QObject::connect(ui->actionOpen_Project, SIGNAL(triggered()), this, SLOT(loadMTCproject()));


    QObject::connect(ui->up_1Byte_pushButton, SIGNAL(pressed()), this, SLOT(move_up1Byte()));
    QObject::connect(ui->down_1Byte_pushButton, SIGNAL(pressed()), this, SLOT(move_down1Byte()));
    QObject::connect(ui->up_1row_pushButton, SIGNAL(pressed()), this, SLOT(move_up1row()));
    QObject::connect(ui->down_1row_pushButton, SIGNAL(pressed()), this, SLOT(move_down1row()));
    QObject::connect(ui->up_1tile_pushButton, SIGNAL(pressed()), this, SLOT(move_up1tile()));
    QObject::connect(ui->down_1tile_pushButton, SIGNAL(pressed()), this, SLOT(move_down1tile()));
    QObject::connect(ui->up_16tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_up16tiles()));
    QObject::connect(ui->down_16tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_down16tiles()));
    QObject::connect(ui->up_160tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_up96tiles()));
    QObject::connect(ui->down_160tiles_pushButton, SIGNAL(pressed()), this, SLOT(move_down96tiles()));

    QObject::connect(ui->palette_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(change_palette()));
    QObject::connect(ui->palette_pushButton, SIGNAL(pressed()), this, SLOT(add_palette_from_ROM()));
    QObject::connect(ui->bookmark_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(goto_bookmark()));
    QObject::connect(ui->bookmark_pushButton, SIGNAL(pressed()), this, SLOT(add_bookmark()));

    QObject::connect(ui->actionPalette2asm, SIGNAL(triggered()), this, SLOT(palette2asm()));
    QObject::connect(ui->actionTiles2asm, SIGNAL(triggered()), this, SLOT(tile2asm()));
    QObject::connect(ui->actionImage2asm, SIGNAL(triggered()), this, SLOT(BMP2asm()));


    std::cout<<"Init UI..."<<std::endl;

    update_palettes();
    update_bookmarks();

    //ui->offset_lineEdit->setText(QString("%1").arg(real_rom.get_offset(),0,16));
    ui->offset_lineEdit->setText(QString("%1").arg(m_project->getOffset(),0,16));
    apply_offset();

    ui->offset_lineEdit->selectAll();
    ui->palettewidget->set_palette(m_project->getPalette());
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    ui->zoomwidget->set_tile(ui->tileswidget->get_selected_tile());

    ui->tilesScrollBar->setSingleStep(16);//the unit is 1 tile
    ui->tilesScrollBar->setPageStep(160);

    ui->background_palette_radioButton->setChecked(true);
    m_project->setROM(&real_rom);//to be able to read palettes from the rom
    
    change_mode();

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    int response = QMessageBox::question(this, "Quitting",
                     tr("Do you want to quit?\nNon-saved project will be lost!"),
                     tr("Yes"), tr("No"), QString(), 0, 1);
    if ( response == 1 )
    {
        event->ignore();
        return;
    }
    event->accept();
}



bool MainWindow::saveMTCproject()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save MTC project"),m_project->getFilename(),tr("MTC project (*.mtc)"));
    if (fileName!="")
        return m_project->save_project(fileName);
    return false;
}

bool MainWindow::loadMTCproject()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Load an MTC Project"), ".", tr("MTC project (*.mtc)"));
    if (fileName!="")
    {
        MTCbookmark* start=m_project->read_project(fileName);
        if (start==0)
        {
            QMessageBox::information(NULL, "Error.", QString("Error reading ")+fileName);
            return false;
        }
        ui->offset_lineEdit->setText(QString("%1").arg(m_project->getOffset(),0,16));

        //TODO palette part, mode...
        decompressed_rom.set_palette(m_project->getPalette());//m_project->read_project updates only real_rom palette
        apply_offset();
        update_palettes();
        update_bookmarks();
        goto_bookmark(start);
        delete start;

        ui->sprite_palette_radioButton->setChecked(m_project->is_sprite_part_of_palette());

        return true;
    }
    return false;
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
    m_project->setOffset(offset);

    if (currently_showing_decompressed_data())
    {
        return_to_real_rom();
    }

    real_rom.create_tiles(offset);
    ui->Apply_offset_pushButton->setEnabled(false);
    ui->bookmark_pushButton->setEnabled(true);
    ui->palette_pushButton->setEnabled(true);
    ui->tile_offset_label->setText(QString("Tile offset: 0x%1").arg(real_rom.get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));

    //at first we have to disconnect the scrollbar from change_offset_scrollbar, otherwise we will have infinite loop
    ui->tilesScrollBar->disconnect();
    ui->tilesScrollBar->setMaximum(real_rom.get_romlength()/(Tile::tile_size()));//the unit for the scrollbar is 1 tile
    ui->tilesScrollBar->setValue(offset/(Tile::tile_size()));
    QObject::connect(ui->tilesScrollBar, SIGNAL(valueChanged(int)), this, SLOT(change_offset_scrollbar(int)));//re-connect

    ui->tileswidget->repaint();
    ui->zoomwidget->repaint();
    if (!((ui->bookmark_comboBox->currentIndex()>0)&&(m_project->getBookmark(ui->bookmark_comboBox->currentIndex()-1)->get_offset()==offset)))
        ui->bookmark_comboBox->setCurrentIndex(0);//bookmark is "???"

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
    ui->bookmark_pushButton->setEnabled(false);
    ui->palette_pushButton->setEnabled(false);
}

void MainWindow::update_tiles()
{
    std::vector<Tile*>::iterator it;
    for ( it=current_rom_shown->get_tiles()->begin() ; it < current_rom_shown->get_tiles()->end(); it++ )
    {
        (*it)->update_palette(m_project->getPalette());
    }
    ui->tileswidget->set_tiles(current_rom_shown->get_tiles());
    ui->palettewidget->set_palette(m_project->getPalette());
    ui->tile_offset_label->setText(QString("Tile Offset: 0x%1").arg(current_rom_shown->get_offset()+ui->tileswidget->get_selection_number()*Tile::tile_size(),0,16));
    ui->zoomwidget->set_tile(ui->tileswidget->get_selected_tile());
    ui->tileswidget->repaint();
    ui->palettewidget->repaint();
    ui->zoomwidget->repaint();
}

bool MainWindow::loadPaletteFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose palette"), ".", tr("SMS Meka Raw Palette Files (*)"));
    if (fileName!="")
    {
        QString description = QInputDialog::getText(this,"Palette Description", "Enter palette description", QLineEdit::Normal);
        if (description!="")
        {
            m_project->addPalette(description,fileName,true);
            ui->palette_comboBox->addItem(m_project->getPalette()->get_description());
            ui->palette_comboBox->setCurrentIndex(m_project->getPaletteIndex());
            update_tiles();
            return true;
        }
    }
    return false;
}

void MainWindow::add_palette_from_ROM()
{
    QString description = QInputDialog::getText(this,"Palette Description", "Enter palette description", QLineEdit::Normal);
    if (description!="")
    {
        m_project->addPalette(description,m_project->getOffset());
        ui->palette_comboBox->addItem(m_project->getPalette()->get_description());
        ui->palette_comboBox->setCurrentIndex(m_project->getPaletteIndex());
        update_tiles();
    }
}


void MainWindow::change_palette()//when a new palette is selected on the combobox
{
    m_project->setPalette(ui->palette_comboBox->currentIndex());
    m_project->set_sprite_part_of_palette(ui->sprite_palette_radioButton->isChecked());
    m_project->getPalette()->set_colors(ui->sprite_palette_radioButton->isChecked());
    //current_rom_shown->set_palette(m_project->getPalette());
    real_rom.set_palette(m_project->getPalette());
    decompressed_rom.set_palette(m_project->getPalette());
    update_tiles();
}

//get list of palettes from mtcproject (only when reading a MTC file)
void MainWindow::update_palettes()
{
    ui->palette_comboBox->clear();
    std::vector<Palette*>::iterator palette_it;
    for( palette_it = m_project->getPalettes()->begin(); palette_it != m_project->getPalettes()->end(); ++palette_it)
    {
        ui->palette_comboBox->addItem((*palette_it)->get_description());
    }
    ui->palette_comboBox->setCurrentIndex(m_project->getPaletteIndex());
}

//get list of palettes from mtcproject (only when reading a MTC file)
void MainWindow::update_bookmarks()
{
    ui->bookmark_comboBox->clear();
    std::vector<MTCbookmark*>::iterator bookmark_it;
    ui->bookmark_comboBox->addItem("???");//when not on a bookmark, show "???"
    for( bookmark_it = m_project->getBookmarks()->begin(); bookmark_it != m_project->getBookmarks()->end(); ++bookmark_it)
    {
        ui->bookmark_comboBox->addItem(QString("%1: ").arg((*bookmark_it)->get_offset(),0,16)+(*bookmark_it)->get_description());
    }
    ui->bookmark_comboBox->setCurrentIndex(0);//if not on a bookmark
}

void MainWindow::add_bookmark()
{
    QString description = QInputDialog::getText(this,"Bookmark Description", "Enter bookmark description", QLineEdit::Normal);
    if (description!="")
    {
        m_project->add_bookmark(description,m_project->getOffset(),m_project->getMode());
        ui->bookmark_comboBox->addItem(QString("%1: ").arg(m_project->getOffset(),0,16)+description);
        ui->bookmark_comboBox->setCurrentIndex(ui->bookmark_comboBox->count()-1);
        update_tiles();
    }
}

void MainWindow::goto_bookmark(MTCbookmark * bookmark)
{
    if (bookmark==0)
    {
        int bookmark_index=ui->bookmark_comboBox->currentIndex();
        if (bookmark_index>0)//first bookmark is "???" = we are not on a bookmark
        {
            bookmark=m_project->getBookmark(bookmark_index-1);
        }else return;
    }

    long offset=bookmark->get_offset();
    long mode=bookmark->get_mode();
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    switch (mode)
    {
    case 1:
        ui->mode_1bpp_radioButton->setChecked(true);
        break;
    case 2:
        ui->mode_2bpp_radioButton->setChecked(true);
        break;
    case 3:
        ui->mode_3bpp_radioButton->setChecked(true);
        break;
    default:
        ui->mode_4bpp_radioButton->setChecked(true);
        break;
    }
    change_mode();
    apply_offset();

}

bool MainWindow::loadROM()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose ROM"), ".", tr("All ROM (*.sms *.gb *.gg *.sg);;SMS ROM (*.sms);;All Files (*)"));
    if (fileName!="")
    {
        m_project->load_ROM(fileName);
        return apply_offset();
    }
    return false;
}

bool MainWindow::saveROM()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save ROM"),m_project->get_original_ROM_filename()+"_new.sms",tr("SMS ROM (*.sms)"));
    if (fileName!="")
    {
        m_project->save_ROM(fileName);
    }
    return false;
}

bool MainWindow::createIPS()
{
    QString ipsfileName = QFileDialog::getSaveFileName(this, tr("Save ROM"),m_project->get_original_ROM_filename()+"_patch.ips",tr("IPS patch (*.ips)"));
    if (ipsfileName!="")
    {
        return current_rom_shown->createIPS(m_project->get_original_ROM_filename().toStdString(),ipsfileName.toStdString());
    }
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
            ui->statusBar->showMessage("Import OK!");
            return true;
        }
        std::cout<<"Import error..."<<std::endl;
        ui->statusBar->showMessage("Import error...");
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
    m_project->setMode(Tile::number_bpp);
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
                    {
                        std::cout<<"Apply hack: \""<<comment<<"\""<<std::endl;
                        if (!real_rom.set_romdata(address,&data))
                        {
                            std::cerr<<"Modification out of rom: "<<line<<std::endl;
                        }
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
    int size_decompressed=1;
    if (current_rom_shown==&decompressed_rom)
        size_decompressed=decompressed_rom.get_compressed_size();

    DialogCompress dialogCompr(&real_rom,m_project,size_decompressed,this);
    if (dialogCompr.exec())
    {
        //update display
        if (currently_showing_decompressed_data()){
            apply_offset();
            decompress_tiles();
            update_tiles();
        }else{
            update_tiles();
        }
    }
    return true;
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
    long offset=0;
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
        ui->statusBar->showMessage("Reached end of ROM. No more compressed data.");
        offset=real_rom.get_romlength();
        ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
        ui->Apply_offset_pushButton->setEnabled(false);//do not let apply offset button enabled while offset changes
        ui->tile_offset_label->repaint();
        real_rom.set_offset(offset);
        return_to_real_rom();
        return false;
    }
    int nbr_uncompressed_tiles=decompressed_rom.decompress_tiles(&real_rom,offset);
    ui->statusBar->showMessage(QString("Found %1 tiles in %2 bytes.")
                               .arg(nbr_uncompressed_tiles,0,10)
                               .arg(decompressed_rom.get_compressed_size(),0,10));
    decompressed_rom.create_tiles(0);
    show_decompressed_data();
    update_tiles();


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


bool MainWindow::palette2asm()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("ASM Palette"),"palette.inc",tr("ASM files (*.asm *.inc)"));
    if (fileName!="")
        return m_project->getPalette()->save_to_asm(fileName);
    return false;
}

bool MainWindow::tile2asm()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("ASM Tiles"),"tiles.inc",tr("ASM files (*.asm *.inc)"));
    if (fileName!="")
    {
        bool ok;
        int nbr_tiles_max=(real_rom.get_romlength()-real_rom.get_offset())/Tile::tile_size();
        int nb_tiles = QInputDialog::getInt(this, tr("How many tiles to export?"),
                                     tr("Number of tiles to export in ASM (not compressed, using current BPP mode)"), 192, 0, nbr_tiles_max, 1, &ok);
        if (ok)
            return real_rom.rom_data2asm(fileName.toStdString(),real_rom.get_offset(),nb_tiles,Tile::tile_size());
        else return false;
    }
    return false;
}

bool MainWindow::BMP2asm()
{
    QString fileNameBMP = QFileDialog::getOpenFileName(this, tr("Image File"),".",tr("Image files (*.BMP *.bmp *.PNG *.png)"));
    if (fileNameBMP!="")
    {
        Rom rom_tmp(m_project->getPalette());
        rom_tmp.import_BMP(fileNameBMP.toStdString(),Tile::number_bpp,true);//update palette at the same time
        int nbr_tiles_max=rom_tmp.get_romlength()/Tile::tile_size();


        QString fileName = QFileDialog::getSaveFileName(this, tr("ASM Tiles"),"tiles.inc",tr("ASM files (*.asm *.inc)"));
        if (fileName!="")
        {
            bool ok;
            int nb_tiles = QInputDialog::getInt(this, tr("How many tiles to export?"),
                                         tr("Number of tiles to export in ASM (not compressed, using current BPP mode)"), nbr_tiles_max, 0, nbr_tiles_max, 1, &ok);
            if (ok)
                return rom_tmp.rom_data2asm(fileName.toStdString(),0,nb_tiles,Tile::tile_size());
            else return false;
        }
        return false;
    }
    return false;
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
    unsigned long offset=real_rom.get_offset();
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
    unsigned long offset=real_rom.get_offset();
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
    unsigned long offset=real_rom.get_offset();
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
    unsigned long offset=real_rom.get_offset();
    offset+=Tile::tile_size()*16;
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_up96tiles()
{
    //return_to_real_rom();
    long offset=real_rom.get_offset();
    offset-=Tile::tile_size()*96;
    if (offset<0) offset=0;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}

void MainWindow::move_down96tiles()
{
    //return_to_real_rom();
    unsigned long offset=real_rom.get_offset();
    offset+=Tile::tile_size()*96;
    if (offset>=real_rom.get_romlength()-Tile::tile_size()*16)
        offset=real_rom.get_romlength()-Tile::tile_size()*16;
    ui->offset_lineEdit->setText(QString("%1").arg(offset,0,16));
    apply_offset();
}
