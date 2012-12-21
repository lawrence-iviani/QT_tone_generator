#include "ctg_app.h"

CTG_app::CTG_app(int argc,char** argv ):
    QApplication(argc,argv),
    m_clipboardDomDocTimeData(),
    m_settings(COMPANYNAME,PROGRAMNAME)
{
    if (m_settings.contains(CTGKEY_SAVE_PROJ_PATH)) m_projectSavePath=m_settings.value(CTGKEY_SAVE_PROJ_PATH).toString();
    else m_projectSavePath=QDir::homePath();
    if (m_settings.contains(CTGKEY_SAVE_CURVE_PATH)) m_curvesSavePath=m_settings.value(CTGKEY_SAVE_CURVE_PATH).toString();
    else m_curvesSavePath=QDir::homePath();
}

bool CTG_app::setClipboard(QDomDocument doc) {
    if (doc.isNull() || !doc.isDocument() ) return false;
    m_clipboardDomDocTimeData=doc;
    return true;
}
