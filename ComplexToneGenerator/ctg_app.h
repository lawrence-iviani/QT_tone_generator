#ifndef CTG_APP_H
#define CTG_APP_H

#include <QApplication>
#include <QSettings>
#include "timedata/generictimedata.h"


class GenericTimeData;


static const QString CTGKEY_SAVE_PROJ_PATH="ProjectSavePath";
static const QString CTGKEY_SAVE_CURVE_PATH="CurveSavePath";

class CTG_app : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QString projectSavePath READ projectSavePath WRITE setProjectSavePath)
    Q_PROPERTY(QString curvesSavePath READ curvesSavePath WRITE setCurvesSavePath)

public:
    CTG_app(int argc,char** argv );

    QString projectSavePath() {return m_projectSavePath;}
    QString curvesSavePath() {return m_curvesSavePath;}
    void setProjectSavePath(QString path) {m_projectSavePath=path;m_settings.setValue(CTGKEY_SAVE_PROJ_PATH,QVariant(path));}
    void setCurvesSavePath(QString path) {m_curvesSavePath=path;m_settings.setValue(CTGKEY_SAVE_CURVE_PATH,QVariant(path));}

signals:
    
public slots:
    
private:
    QString m_projectSavePath;
    QString m_curvesSavePath;
    GenericTimeData * m_clipboardTimeData;
    QSettings m_settings;
};

#endif // CTG_APP_H
