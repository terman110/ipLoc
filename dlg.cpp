#include "dlg.h"
#include "ui_dlg.h"

dlg::dlg(int* timerval, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlg)
{
    ui->setupUi(this);
    _timerval = timerval;
    ui->spinBox->setValue( *_timerval/1000);
}

dlg::~dlg()
{
    delete ui;
}

void dlg::on_spinBox_editingFinished()
{
    *_timerval = ui->spinBox->value()*1000;
}

void dlg::on_spinBox_valueChanged(int arg1)
{
    *_timerval = arg1*1000;
}
