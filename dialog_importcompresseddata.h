#ifndef DIALOG_IMPORTCOMPRESSEDDATA_H
#define DIALOG_IMPORTCOMPRESSEDDATA_H

#include <QDialog>
#include <string>

namespace Ui {
    class Dialog_ImportCompressedData;
}

class Dialog_ImportCompressedData : public QDialog {
    Q_OBJECT
public:
    Dialog_ImportCompressedData(QWidget *parent = 0);
    ~Dialog_ImportCompressedData();

    std::string get_filename();
    unsigned long get_adress();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Dialog_ImportCompressedData *ui;

public slots:
    void select_dat_file();
};

#endif // DIALOG_IMPORTCOMPRESSEDDATA_H
