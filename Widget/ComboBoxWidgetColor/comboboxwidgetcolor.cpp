#include "comboboxwidgetcolor.h"
#include "ui_comboboxwidgetcolor.h"

const QString ComboBoxWidgetColor::BLACK="Black";
const QString ComboBoxWidgetColor::WHITE="White";
const QString ComboBoxWidgetColor::RED="Red";
const QString ComboBoxWidgetColor::DARKRED="Dark red";
const QString ComboBoxWidgetColor::GREEN="Green";
const QString ComboBoxWidgetColor::DARKGREEN="Dark green";
const QString ComboBoxWidgetColor::BLUE="Blue";
const QString ComboBoxWidgetColor::DARKBLUE="Dark blue";
const QString ComboBoxWidgetColor::CYAN="Cyan";
const QString ComboBoxWidgetColor::DARKCYAN="Dark cyan";
const QString ComboBoxWidgetColor::MAGENTA="Magenta";
const QString ComboBoxWidgetColor::DARKMAGENTA="Dark magenta";
const QString ComboBoxWidgetColor::YELLOW="Yellow";
const QString ComboBoxWidgetColor::DARKYELLOW="Dark yellow";
const QString ComboBoxWidgetColor::GRAY="Gray";
const QString ComboBoxWidgetColor::DARKGRAY="Dark gray";
const QString ComboBoxWidgetColor::LIGHTGRAY="Light gray";
const QString ComboBoxWidgetColor::UNDEFINED="Unknonwn color";

ComboBoxWidgetColor::ComboBoxWidgetColor(QWidget  *parent) :
    ComboBoxNameWidget(parent),
    ui(new Ui::ComboBoxWidgetColor)
{
    ui->setupUi(this);
    this->addItems(ComboBoxWidgetColor::getColorsNameList() );
    this->setText("Select Base Color");
    connect(this,SIGNAL(currentIndexChanged(int)),this,SLOT(valueChanged(int)));
}

ComboBoxWidgetColor::~ComboBoxWidgetColor()
{
    delete ui;
}

void ComboBoxWidgetColor::setColor(QString colorName) {
    QStringList l=ComboBoxWidgetColor::getColorsNameList();
    int index=l.indexOf(colorName);
    this->setIndex(index);
}

void ComboBoxWidgetColor::setColor(QColor color) {
    this->setColor(ComboBoxWidgetColor::convertQtColorToString(color));
}

void ComboBoxWidgetColor::valueChanged(int index) {
    QString v=this->item();
    if (ComboBoxWidgetColor::isSupportedColorName(v)) {
        emit colorChanged(ComboBoxWidgetColor::convertStringToQtColor(v));
    }
}

QString ComboBoxWidgetColor::convertQtColorToString(QColor color) {

    if (color==Qt::black)  return ComboBoxWidgetColor::BLACK;
    if (color== Qt::white)  return ComboBoxWidgetColor::WHITE;
    if (color==Qt::red)  return ComboBoxWidgetColor::RED;
    if (color==Qt::darkRed)  return ComboBoxWidgetColor::DARKRED;
    if (color==Qt::green)  return ComboBoxWidgetColor::GREEN;
    if (color==Qt::darkGreen)   return ComboBoxWidgetColor::DARKGREEN;
    if (color==Qt::blue)  return ComboBoxWidgetColor::BLUE;
    if (color==Qt::darkBlue)  return ComboBoxWidgetColor::DARKBLUE;
    if (color==Qt::cyan)  return ComboBoxWidgetColor::CYAN;
    if (color==Qt::darkCyan)  return ComboBoxWidgetColor::DARKCYAN;
    if (color==Qt::magenta)  return ComboBoxWidgetColor::MAGENTA;
    if (color==Qt::darkMagenta)  return ComboBoxWidgetColor::DARKMAGENTA;
    if (color==Qt::yellow)  return ComboBoxWidgetColor::YELLOW;
    if (color==Qt::darkYellow)   return ComboBoxWidgetColor::DARKYELLOW;
    if (color==Qt::gray)  return ComboBoxWidgetColor::GRAY;
    if (color==Qt::darkGray)  return ComboBoxWidgetColor::DARKGRAY;
    if (color==Qt::lightGray)  return ComboBoxWidgetColor::LIGHTGRAY;

    return ComboBoxWidgetColor::UNDEFINED;
}

QColor ComboBoxWidgetColor::convertStringToQtColor(QString colorName) {
    if (colorName==ComboBoxWidgetColor::BLACK) return Qt::black;
    if (colorName==ComboBoxWidgetColor::WHITE) return Qt::white;
    if (colorName==ComboBoxWidgetColor::RED) return Qt::red;
    if (colorName==ComboBoxWidgetColor::DARKRED) return  Qt::darkRed;
    if (colorName==ComboBoxWidgetColor::GREEN) return Qt::green;
    if (colorName==ComboBoxWidgetColor::DARKGREEN) return Qt::darkGreen;
    if (colorName==ComboBoxWidgetColor::BLUE) return Qt::blue;
    if (colorName==ComboBoxWidgetColor::DARKBLUE) return Qt::darkBlue;
    if (colorName==ComboBoxWidgetColor::CYAN) return Qt::cyan;
    if (colorName==ComboBoxWidgetColor::DARKCYAN) return  Qt::darkCyan;
    if (colorName==ComboBoxWidgetColor::MAGENTA) return Qt::magenta;
    if (colorName==ComboBoxWidgetColor::DARKMAGENTA) return Qt::darkMagenta;
    if (colorName==ComboBoxWidgetColor::YELLOW) return Qt::yellow;
    if (colorName==ComboBoxWidgetColor::DARKYELLOW) return Qt::darkYellow;
    if (colorName==ComboBoxWidgetColor::GRAY )return Qt::gray;
    if (colorName==ComboBoxWidgetColor::DARKGRAY) return Qt::darkGray;
    if (colorName==ComboBoxWidgetColor::LIGHTGRAY) return Qt::lightGray;
    return *( new QColor());
}

bool ComboBoxWidgetColor::isSupportedColorName(QString colorName) {
    if (colorName==ComboBoxWidgetColor::BLACK) return true;
    if (colorName==ComboBoxWidgetColor::WHITE) return true;
    if (colorName==ComboBoxWidgetColor::RED) return true;
    if (colorName==ComboBoxWidgetColor::DARKRED) return true;
    if (colorName==ComboBoxWidgetColor::GREEN) return true;
    if (colorName==ComboBoxWidgetColor::DARKGREEN) return true;
    if (colorName==ComboBoxWidgetColor::BLUE) return true;
    if (colorName==ComboBoxWidgetColor::DARKBLUE) return true;
    if (colorName==ComboBoxWidgetColor::CYAN) return true;
    if (colorName==ComboBoxWidgetColor::DARKCYAN) return true;
    if (colorName==ComboBoxWidgetColor::MAGENTA) return true;
    if (colorName==ComboBoxWidgetColor::DARKMAGENTA) return true;
    if (colorName==ComboBoxWidgetColor::YELLOW) return true;
    if (colorName==ComboBoxWidgetColor::DARKYELLOW) return true;
    if (colorName==ComboBoxWidgetColor::GRAY ) return true;
    if (colorName==ComboBoxWidgetColor::DARKGRAY) return true;
    if (colorName==ComboBoxWidgetColor::LIGHTGRAY) return true;
    return false;
}

bool ComboBoxWidgetColor::isSupportedColorName(QColor color) {
    if (color==Qt::black) return true;
    if (color==Qt::white) return true;
    if (color==Qt::red) return true;
    if (color==Qt::darkRed) return true;
    if (color==Qt::green) return true;
    if (color==Qt::darkGreen) return true;
    if (color==Qt::blue) return true;
    if (color==Qt::darkBlue) return true;
    if (color==Qt::cyan) return true;
    if (color==Qt::darkCyan) return true;
    if (color==Qt::magenta) return true;
    if (color==Qt::darkMagenta) return true;
    if (color==Qt::yellow) return true;
    if (color==Qt::darkYellow) return true;
    if (color==Qt::gray ) return true;
    if (color==Qt::darkGray) return true;
    if (color==Qt::lightGray) return true;
    return false;
}

QStringList  ComboBoxWidgetColor::getColorsNameList() {
    QStringList l= *(new QStringList());
    l << ComboBoxWidgetColor::BLACK << ComboBoxWidgetColor::WHITE << ComboBoxWidgetColor::RED << ComboBoxWidgetColor::DARKRED  << ComboBoxWidgetColor::GREEN;
    l << ComboBoxWidgetColor::DARKGREEN << ComboBoxWidgetColor::BLUE << ComboBoxWidgetColor::DARKBLUE << ComboBoxWidgetColor::CYAN << ComboBoxWidgetColor::DARKCYAN;
    l << ComboBoxWidgetColor::MAGENTA << ComboBoxWidgetColor::DARKMAGENTA << ComboBoxWidgetColor::YELLOW << ComboBoxWidgetColor::DARKYELLOW;
    l << ComboBoxWidgetColor::GRAY << ComboBoxWidgetColor::DARKGRAY << ComboBoxWidgetColor::LIGHTGRAY;
    return l;
}
