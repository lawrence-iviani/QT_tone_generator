#include "selectcurvewindowhelper.h"

SelectCurveWindowHelper::SelectCurveWindowHelper(QObject *parent) :
    QObject(parent)

{

}

SelectCurveWindowHelper::~SelectCurveWindowHelper() {
    m_dataList.clear();
}

void SelectCurveWindowHelper::addData(struct S_DataCurve d) {
    m_dataList.append(d);
    emit dataChanged();
}

QStringList SelectCurveWindowHelper::getNames() {
    QStringList * retval=new QStringList();
    S_DataCurve t;
    for (int n=0; n < m_dataList.length(); n++)  {
        t=m_dataList.at(n);
        retval->append(t.name);
    }
    return *retval;
}

QString SelectCurveWindowHelper::getName(int index) {
    return this->getItem(index).name;
}

QString SelectCurveWindowHelper::getDescription(int index) {
    return this->getItem(index).description;
}

void SelectCurveWindowHelper::setDataCurveIndex(int index) {
    m_selectedDataCurve=this->getItem(index);
}

S_DataCurve SelectCurveWindowHelper::getSelectedDataCurve() {
    return m_selectedDataCurve;
}

S_DataCurve SelectCurveWindowHelper::getItem(int index) {
    S_DataCurve retval;
    if ( (index >=0) && (index <= m_dataList.length())) {
        retval=m_dataList.at(index);
    }
    return retval;
}
