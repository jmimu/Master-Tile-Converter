#include "dialog_importcompresseddata.h"
#include "ui_dialog_importcompresseddata.h"

#include <QFileDialog>
#include <iostream>

Dialog_ImportCompressedData::Dialog_ImportCompressedData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ImportCompressedData)
{
    ui->setupUi(this);
    QObject::connect(ui->pushButton_datfilename, SIGNAL(clicked()), this, SLOT(select_dat_file()));
}

Dialog_ImportCompressedData::~Dialog_ImportCompressedData()
{
    delete ui;
}

void Dialog_ImportCompressedData::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dialog_ImportCompressedData::select_dat_file()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose data file"), ".", tr("Tile Decoder compressed data (*.dat);;All files (*)"));
    if (fileName!="")
    {
        ui->lineEdit_datfile->setText(fileName);
    }
}

std::string Dialog_ImportCompressedData::get_filename()
{
    return ui->lineEdit_datfile->text().toStdString();
}

unsigned long Dialog_ImportCompressedData::get_adress()
{
    bool ok;
    unsigned long adress= ui->lineEdit_adress->text().toULong(&ok,16);
    if (!ok)
    {
        std::cerr<<"Error in adress!"<<std::endl;
        return 0;
    }
    return adress;
}
