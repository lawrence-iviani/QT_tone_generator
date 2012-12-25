#ifndef SELECTMULTIPLECURVESWINDOWDIALOG_H
#define SELECTMULTIPLECURVESWINDOWDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include "ui_selectmultiplecurveswindowdialog.h"


namespace Ui {
class SelectMultipleCurvesWindowDialog;
}

class SelectMultipleCurvesWindowDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SelectMultipleCurvesWindowDialog(QStringList *stringList, QWidget *parent = 0);

    ~SelectMultipleCurvesWindowDialog();
    QList<int> getSelectedCurvesIndex() {return m_selectedItems;}
    QString actionDialog() {return m_actionText;}

public slots:
    void selectedButtonPressed();
    void backButtonPressed();
    void setActionDialog(const QString& title);
    
private:
    Ui_SelectMultipleCurvesWindowDialog *ui;
    QStringListModel *m_model;
    QList<int> m_selectedItems;
    QString m_actionText;
};

#endif // SELECTMULTIPLECURVESWINDOWDIALOG_H
