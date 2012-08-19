#ifndef SELECTCURVEWINDOWDIALOG_H
#define SELECTCURVEWINDOWDIALOG_H

#include <QDialog>
#include "selectcurvewindowhelper.h"

namespace Ui {
class SelectCurveWindowDialog;
}

class SelectCurveWindowDialog : public QDialog
{
    Q_OBJECT
    
public:
    SelectCurveWindowDialog( SelectCurveWindowHelper * h, QWidget *parent = 0);
    ~SelectCurveWindowDialog();
    static SelectCurveWindowHelper * getDialogCurveHelper();
public slots:
    void changedCurve(int index);
    void cancelSelect();
private:
    Ui::SelectCurveWindowDialog *ui;
    SelectCurveWindowHelper * m_helper;
    S_DataCurve m_curveSelected;
};

#endif // SELECTCURVEWINDOWDIALOG_H
