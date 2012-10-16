#ifndef SELECTREMOVECURVEWINDOWDIALOG_H
#define SELECTREMOVECURVEWINDOWDIALOG_H

#include <QStringListModel>
#include <QDialog>

namespace Ui {
class SelectRemoveCurveWindowDialog;
}

class SelectRemoveCurveWindowDialog : public QDialog
{
    Q_OBJECT
    
public:
    SelectRemoveCurveWindowDialog( QStringList *stringList, QWidget *parent = 0);
    QList<int> getRemoveCurvesIndex() {return m_selectedItems;}
    ~SelectRemoveCurveWindowDialog();

public slots:
    void removeButtonPressed();
    void backButtonPressed();
private:
    Ui::SelectRemoveCurveWindowDialog *ui;
    QStringListModel *m_model;
    QList<int> m_selectedItems;
};

#endif // SELECTREMOVECURVEWINDOWDIALOG_H
