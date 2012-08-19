#include "selectcurvewindowdialog.h"
#include "ui_selectcurvewindowdialog.h"

SelectCurveWindowDialog::SelectCurveWindowDialog(SelectCurveWindowHelper * h, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCurveWindowDialog)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );

    m_helper=h;
    ui->comboBox->addItems(m_helper->getNames());
    this->changedCurve(ui->comboBox->currentIndex());
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changedCurve(int)));
}

SelectCurveWindowDialog::~SelectCurveWindowDialog()
{
    delete ui;
}

void SelectCurveWindowDialog::changedCurve(int index) {
    m_helper->setDataCurveIndex(index);
    ui->label->setText(m_helper->getSelectedDataCurve().description);
}

void SelectCurveWindowDialog::cancelSelect() {
    m_helper->setDataCurveIndex(-1);
    this->reject();
}

SelectCurveWindowHelper * SelectCurveWindowDialog::getDialogCurveHelper() {
    SelectCurveWindowHelper * t=new SelectCurveWindowHelper();
    return t;
}
