#ifndef DLG_H
#define DLG_H

#include <QtWidgets/QDialog>

namespace Ui {
    class dlg;
}

class dlg : public QDialog
{
    Q_OBJECT

public:
    explicit dlg(int* timerval, QWidget *parent = 0);
    ~dlg();

private slots:
    void on_spinBox_editingFinished();
    void on_spinBox_valueChanged(int arg1);

private:
    int* _timerval;
    Ui::dlg *ui;
};

#endif // DLG_H
