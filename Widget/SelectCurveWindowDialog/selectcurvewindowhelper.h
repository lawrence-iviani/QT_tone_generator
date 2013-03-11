#ifndef SELECTCURVEWINDOWHELPER_H
#define SELECTCURVEWINDOWHELPER_H

#include <QObject>
#include <QList>
#include <QStringList>

struct s_dataCurve {
    QString className;
    QString name;
    QString description;
};

/**
  * This class is a data helper class to store information about curve selection, it needs to be init adding data curve information,  it contains any info and
  * update made with the relative curve selection dialog.
  */

class SelectCurveWindowHelper : public QObject
{
    Q_OBJECT
public:
    explicit SelectCurveWindowHelper(QObject *parent = 0);  
    ~SelectCurveWindowHelper();
    QStringList getNames(); //Populate a list of the curves name contained in the class. Useful to populate the dialog combo box
    QString getName(int index); //Get the name at index
    QString getDescription(int index); //Get the description at index
    s_dataCurve getSelectedDataCurve(); //Give back the last data curve selected.

signals:
    void dataChanged();
public slots:
    void addData(struct s_dataCurve data);
    void setDataCurveIndex(int index);
private:
    QList<s_dataCurve> m_dataList;
    s_dataCurve getItem(int index);
    s_dataCurve m_selectedDataCurve;//The data
};

#endif // SELECTCURVEWINDOWHELPER_H
