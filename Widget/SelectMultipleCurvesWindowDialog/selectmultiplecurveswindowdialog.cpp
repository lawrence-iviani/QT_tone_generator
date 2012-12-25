#include "selectmultiplecurveswindowdialog.h"


SelectMultipleCurvesWindowDialog::SelectMultipleCurvesWindowDialog(QStringList * stringList,QWidget *parent) :
    QDialog(parent),
    ui(new Ui_SelectMultipleCurvesWindowDialog),
    m_selectedItems()
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    m_model = new QStringListModel((const QStringList) *stringList);
    ui->listViewCurves->setModel(m_model);
    ui->listViewCurves->setSelectionMode(QAbstractItemView::ExtendedSelection);//Multiple selection
    ui->listViewCurves->setEditTriggers(QAbstractItemView::NoEditTriggers);//No editable items
    connect(ui->pushButtonBack,SIGNAL(clicked()),this,SLOT(backButtonPressed()));
    connect(ui->pushButtonOK,SIGNAL(clicked()),this,SLOT(selectedButtonPressed()));
    setActionDialog("...");
}

SelectMultipleCurvesWindowDialog::~SelectMultipleCurvesWindowDialog()
{
    delete ui;
}

void SelectMultipleCurvesWindowDialog::selectedButtonPressed() {
    foreach(const QModelIndex &index, ui->listViewCurves->selectionModel()->selectedIndexes())
           m_selectedItems.append(index.row());

    this->close();
}

void SelectMultipleCurvesWindowDialog::backButtonPressed() {
    this->close();
}

void SelectMultipleCurvesWindowDialog::setActionDialog(const QString &title) {
    m_actionText=title;
    QString _labelText=QString("Select curves to %1").arg(title);
    QString _titleText=QString(title).append(" curves dialog");
    ui->titleLabel->setText(_labelText);
    setWindowTitle(_titleText);
}

