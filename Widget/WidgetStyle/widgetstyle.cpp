#include "widgetstyle.h"


WidgetStyle::WidgetStyle(const QString sheetName)
{
    m_style=(QWindowsStyle*)new  QCleanlooksStyle();// set a default style
    QString _fileName(":/stylesheet/"+ sheetName + ".qss");
    loadStyleSheet(_fileName);
}

WidgetStyle::~WidgetStyle() {
  //  delete m_style;
}

void WidgetStyle::setStyleSheet(const QString &sheetName) {

    QString _fileName(":/stylesheet/"+ sheetName + ".qss");
    loadStyleSheet(_fileName);
}

void WidgetStyle::loadStyleSheet(const QString &sheetName)
{
    QFile file(sheetName);
    file.open(QFile::ReadOnly);
    Q_ASSERT(file.isOpen());
    if (!file.isOpen()) {
        qWarning() << "WidgetStyle::loadStyleSheet invalid qss";
    } else  m_styleSheet = QLatin1String(file.readAll());
}

void WidgetStyle::setStyle(QWidget * w)
{
    w->setStyle(m_style);
    w->setStyleSheet(m_styleSheet);
}

void WidgetStyle::setStyle(QApplication * app)
{
    app->setStyle(m_style);
    app->setStyleSheet(m_styleSheet);
}
