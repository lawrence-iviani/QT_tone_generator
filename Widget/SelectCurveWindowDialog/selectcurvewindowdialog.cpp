#include "selectcurvewindowdialog.h"
#include "ui_selectcurvewindowdialog.h"

SelectCurveWindowDialog::SelectCurveWindowDialog(const QMap<QString, s_curveDescriptionData> *mapItems, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectCurveWindowDialog),
    m_pMap(mapItems)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );

    QMap<QString, s_curveDescriptionData>::const_iterator i = m_pMap->begin();
    while (i != m_pMap->end()) {
        s_curveDescriptionData _s= i.value();
        //i.value()
         ui->comboBox->addItem(_s.name); //i.value().name);
         ++i;
     }
    //ui->comboBox->addItems(m_helper->getItemsName());
    this->changedCurve(ui->comboBox->currentIndex());
    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(changedCurve(int)));
}

SelectCurveWindowDialog::~SelectCurveWindowDialog()
{ delete ui;}

void SelectCurveWindowDialog::changedCurve(int index) {
    Q_UNUSED(index);
    m_curveSelected=ui->comboBox->currentText();
    QString _newName=m_pMap->find(m_curveSelected).value().name;
    ui->label->setText(_newName);
}

void SelectCurveWindowDialog::cancelSelect() {
    this->reject();
}

