#include "selectremovecurvewindowdialog.h"
#include "ui_selectremovecurvewindowdialog.h"

SelectRemoveCurveWindowDialog::SelectRemoveCurveWindowDialog(QStringList * stringList,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectRemoveCurveWindowDialog),
    m_selectedItems()
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    m_model = new QStringListModel((const QStringList) *stringList);
    ui->listViewCurves->setModel(m_model);
    ui->listViewCurves->setSelectionMode(QAbstractItemView::ExtendedSelection);//Multiple selection
    ui->listViewCurves->setEditTriggers(QAbstractItemView::NoEditTriggers);//No editable items

}

SelectRemoveCurveWindowDialog::~SelectRemoveCurveWindowDialog()
{
    delete ui;
}

void SelectRemoveCurveWindowDialog::removeButtonPressed() {
    foreach(const QModelIndex &index, ui->listViewCurves->selectionModel()->selectedIndexes())
           m_selectedItems.append(index.row());
    this->close();
}

void SelectRemoveCurveWindowDialog::backButtonPressed() {
    this->close();
}


