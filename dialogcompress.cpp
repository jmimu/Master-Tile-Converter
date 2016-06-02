#include "dialogcompress.h"
#include "ui_dialogcompress.h"

#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qinputdialog.h>

DialogCompress::DialogCompress(Rom *_Rom, MTCproject *_Project, int size_decompressed, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCompress),
    mRom(_Rom),mProject(_Project),mRomTmp(mProject->getPalette())
{
    ui->setupUi(this);
    QObject::connect(ui->pushButtonBMPBrowse, SIGNAL(pressed()), this, SLOT(chooseBMP()));
    QObject::connect(ui->pushButtonComprBrowse, SIGNAL(pressed()), this, SLOT(chooseOutput()));
    QObject::connect(ui->pushButtonCompress, SIGNAL(pressed()), this, SLOT(compress()));
    QObject::connect(ui->groupBoxInsert, SIGNAL(clicked()), this, SLOT(updateCompressButton()));
    QObject::connect(ui->groupBoxSave, SIGNAL(clicked()), this, SLOT(updateCompressButton()));
    QObject::connect(ui->lineEditComprFile, SIGNAL(textChanged(const QString)), this, SLOT(updateCompressButton()));
    ui->spinBoxAddress->setValue(mRom->get_offset());
    ui->spinBoxNbTiles->setValue(1);
    ui->spinBoxFixedSize->setValue(size_decompressed);
    ui->groupBoxSize->setChecked(false);
    ui->groupBoxSave->setChecked(false);
    updateCompressButton();
}

DialogCompress::~DialogCompress()
{
    delete ui;
}

bool DialogCompress::updateCompressButton()
{
    ui->pushButtonCompress->setEnabled(
                ui->groupBoxInsert->isChecked()
                || (
                    ui->groupBoxSave->isChecked()
                    &&(ui->lineEditComprFile->text().size()>0)
                   )
                );
}

bool DialogCompress::chooseBMP()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose BMP file"), ".", tr("BMP File (*.bmp)"));
    if (fileName!="")
    {
        if (mRomTmp.import_BMP(fileName.toStdString(),4))
        {
            bool ok;
            unsigned long nbr_total_tiles=mRomTmp.get_romlength()/4/8;
            ui->spinBoxNbTiles->setValue(nbr_total_tiles);
            ui->spinBoxNbTiles->setMaximum(nbr_total_tiles);
            ui->lineEditBMPFile->setText(fileName);
            return true;
        }else{
            QMessageBox::warning(this,"Reading Error","Error reading BMP file!");
            return false;
        }
    }
    return false;
}

bool DialogCompress::chooseOutput()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Choose compressed DAT file name"), ".", tr("DAT File (*.dat)"));
    if (fileName!="")
    {
        ui->lineEditComprFile->setText(fileName);
        return true;
    }
    return false;
}


bool DialogCompress::compress()
{
    QString outFileName="compr_tmp.dat";
    if (ui->groupBoxSave->isChecked())
        outFileName=ui->lineEditComprFile->text();

    int nb_tiles=ui->spinBoxNbTiles->value();
    int target_size=-1;
    if (ui->groupBoxSize->isChecked()) target_size=ui->spinBoxFixedSize->value();
    unsigned long nb_bytes=mRomTmp.compress_tiles(nb_tiles,outFileName.toStdString(),target_size);

    QString message;
    if ((ui->groupBoxSize->isChecked())&&(nb_bytes>ui->spinBoxFixedSize->value()))
    {
        message=QString("The %1 tiles have been compressed, but its size is %2 bytes instead of %3!!!").arg(nb_tiles).arg(nb_bytes).arg(target_size);

        if (ui->groupBoxInsert->isChecked())
        {
            message+="\nInsertion Cancelled.";
        }
        QMessageBox::warning(this, "Compression error", message);
        return false;
    }

    if (ui->groupBoxInsert->isChecked())
    {
        long offset=ui->spinBoxAddress->value();
        if (offset>=mRom->get_romlength())
        {
            QMessageBox::warning(this,"Out of ROM","Error: selected address is out of ROM!");
            return false;
        }
        //part from import_rawdata
        if (mRom->import_rawdata(outFileName.toStdString(),offset))
        {
            QMessageBox::information(this, "Compression done.", QString("The %1 tiles have been compressed and inserted. Its size is %2 bytes").arg(nb_tiles).arg(nb_bytes));
            accept();
            return true;
        }else{
            return false;
        }
    }else{
        QMessageBox::information(this, "Compression done.", QString("The %1 tiles have been compressed. Its size is %2 bytes").arg(nb_tiles).arg(nb_bytes));
    }
    return true;
}
