#ifndef COMBOBOXWIDGETCOLOR_H
#define COMBOBOXWIDGETCOLOR_H

#include <QWidget>
#include "comboboxnamewidget.h"

namespace Ui {
class ComboBoxWidgetColor;
}

class ComboBoxWidgetColor : public ComboBoxNameWidget
{
    Q_OBJECT

public:
    static const QString BLACK;
    static const QString WHITE;
    static const QString RED;
    static const QString DARKRED;
    static const QString GREEN;
    static const QString DARKGREEN;
    static const QString BLUE;
    static const QString DARKBLUE;
    static const QString CYAN;
    static const QString DARKCYAN;
    static const QString MAGENTA;
    static const QString DARKMAGENTA;
    static const QString YELLOW;
    static const QString DARKYELLOW;
    static const QString GRAY;
    static const QString DARKGRAY;
    static const QString LIGHTGRAY;
    static const QString UNDEFINED;

    explicit ComboBoxWidgetColor(QWidget *parent = 0);
    ~ComboBoxWidgetColor();


    static QStringList  getColorsNameList();
    static QString convertQtColorToString(QColor color);
    static QColor convertStringToQtColor(QString colorName);
    static bool isSupportedColorName(QString colorName);
    static bool isSupportedColorName(QColor color);

    QColor color() {return ComboBoxWidgetColor::convertStringToQtColor(this->item());}
    QString colorName();

    /**
     * @brief getNextColor Generate another color baesd on the actual color
     * @param color the actual color
     * @return  the new color
     */
    static QColor getNextColor(QColor &color);

public slots:
    void setColor(QColor color);
    void setColor(QString colorName);
    void valueChanged(int index);
signals:
    void colorChanged(QColor newColor);
private:
    Ui::ComboBoxWidgetColor *ui;
};





#endif // COMBOBOXWIDGETCOLOR_H
