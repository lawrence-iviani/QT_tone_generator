#ifndef CTG_APP_H
#define CTG_APP_H

#include <QApplication>
#include <QSettings>
#include <QDomDocument>
#include <QDir>
#include "CTG_constants.h"



class QDomDocument;


static const QString CTGKEY_SAVE_PROJ_PATH="ProjectSavePath";
static const QString CTGKEY_SAVE_CURVE_PATH="CurveSavePath";
static const QString CTGKEY_SAVE_AUDIO_DIGEST_PATH="CurveSavePath";

class CTG_app : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QString projectSavePath READ projectSavePath WRITE setProjectSavePath)
    Q_PROPERTY(QString projectName READ projectName WRITE setProjectName)
    Q_PROPERTY(QString curveSavePath READ curveSavePath WRITE setCurveSavePath)
    Q_PROPERTY(QString audioDigestSavePath READ audioDigestSavePath WRITE setAudioDigestSavePath)
    Q_PROPERTY(QString audioDigestSaveName READ audioDigestSaveName WRITE setAudioDigestSaveName)

public:
    CTG_app(int argc,char** argv );

    //GETTER
    QString projectSavePath() {return m_projectSavePath;}
    QString projectName() {return m_projectName;}
    QString curveSavePath() {return m_curvesSavePath;}
    QString audioDigestSavePath() {return m_audioDigestSavePath;}
    QString audioDigestSaveName() {return m_audioDigestName;}

    //SETTER
    void setProjectSavePath(QString path) {m_projectSavePath=path;m_settings.setValue(CTGKEY_SAVE_PROJ_PATH,QVariant(path));}
    void setProjectName(QString name) {m_projectName=name;}
    void setCurveSavePath(QString path) {m_curvesSavePath=path;m_settings.setValue(CTGKEY_SAVE_CURVE_PATH,QVariant(path));}
    void setAudioDigestSavePath(QString path) {m_audioDigestSavePath=path;m_settings.setValue(CTGKEY_SAVE_AUDIO_DIGEST_PATH,QVariant(path));}
    void setAudioDigestSaveName(QString name) {m_audioDigestName=name;}

    bool setClipboard(QDomDocument doc);
    QDomDocument& clipboard() {return m_clipboardDomDocTimeData;}
signals:
    
public slots:
    
private:
    QString m_projectSavePath;
    QString m_projectName;
    QString m_curvesSavePath;
    QString m_audioDigestSavePath;
    QString m_audioDigestName;

    QDomDocument m_clipboardDomDocTimeData;
    QSettings m_settings;
};

#endif // CTG_APP_H
