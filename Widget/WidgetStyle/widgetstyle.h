#ifndef WIDGETSTYLE_H
#define WIDGETSTYLE_H

#include "WidgetStyle_global.h"
#include <QStyle>
#include <QCleanlooksStyle>
#include <QWidget>
#include <QFile>
#include <QApplication>
#include <QDebug>

class WIDGETSTYLESHARED_EXPORT WidgetStyle
{
public:

    WidgetStyle(const QString sheetName);
    virtual ~WidgetStyle();

    void setStyle(QWidget *w);
    void setStyle(QApplication * app);
    void setStyleSheet(const QString &sheetName) ;

private:
    QString m_styleSheet;
    QWindowsStyle * m_style;

    void loadStyleSheet(const QString &sheetName);

};

#endif // WIDGETSTYLE_H
