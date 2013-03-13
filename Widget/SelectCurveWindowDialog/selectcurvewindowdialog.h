#ifndef SELECTCURVEWINDOWDIALOG_H
#define SELECTCURVEWINDOWDIALOG_H

#include <QDialog>

struct s_curveDescriptionData {
    QString className;
    QString name;
    QString description;
};

namespace Ui {
class SelectCurveWindowDialog;
}

class SelectCurveWindowDialog : public QDialog
{
    Q_OBJECT
    
public:
    SelectCurveWindowDialog( const QMap<QString,s_curveDescriptionData>* pMap, QWidget *parent = 0);
    ~SelectCurveWindowDialog();
    QString getSelectedCurveName() {return m_curveSelected;}
public slots:
    void changedCurve(int index);
    void cancelSelect();
private:
    Ui::SelectCurveWindowDialog *ui;
    const QMap<QString,s_curveDescriptionData>* m_pMap;
    QString m_curveSelected;
};

#endif // SELECTCURVEWINDOWDIALOG_H
