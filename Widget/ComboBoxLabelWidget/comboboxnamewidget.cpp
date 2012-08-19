#include "comboboxnamewidget.h"
#include "ui_comboboxnamewidget.h"

ComboBoxNameWidget::ComboBoxNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ComboBoxNameWidget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout);
    QFont f=*(new QFont());
    f.setPointSize(10);
    this->setFont(f);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SIGNAL(currentIndexChanged(int)));
}

ComboBoxNameWidget::~ComboBoxNameWidget()
{
    delete ui;
}

void ComboBoxNameWidget::addItem(const QString &item){
    ui->comboBox->addItem(item);
}

void ComboBoxNameWidget::addItems(const QStringList &  items){
    ui->comboBox->addItems(items);
}

void ComboBoxNameWidget::removeItem(int index){
    ui->comboBox->removeItem(index);
}

void ComboBoxNameWidget::setIndex(int index){
    ui->comboBox->setCurrentIndex(index);
}

QString ComboBoxNameWidget::item(){
    return ui->comboBox->currentText();
}

int ComboBoxNameWidget::index() {
    return ui->comboBox->currentIndex();
}

void ComboBoxNameWidget::setText(const QString &text) {
    ui->label->setText(text);
}

QString ComboBoxNameWidget::text() {return ui->label->text();}

void ComboBoxNameWidget::setFont(QFont  f) {
    QWidget::setFont(f);
    ui->comboBox->setFont(f);
    f.setPointSize(f.pointSize()-1);
    ui->label->setFont(f);
}

