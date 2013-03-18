#ifndef FREQPLOTWIDGETUI_H
#define FREQPLOTWIDGETUI_H

#include <DataUiHandler.h>
#include <QLayout>
#include <QComboBox>
#include "freqplotwidget.h"
#include "zmp_handler.h"

class FreqPlotWidgetUI : public DataUiHandlerUI
{
    Q_OBJECT
public:
    explicit FreqPlotWidgetUI(QWidget *widget = 0);
    virtual ~FreqPlotWidgetUI();
signals:
    void binsNumberUIChanged(uint);
    void windowTypeUIChanged(QString);
    void overlapTypeUIChanged(qreal);

public slots:
    void binsNumberUIUpdate(uint nBins);
    void windowTypeUIUpdate(QString windowName);
    void overlapUIUpdate(qreal overlap);

private slots:
    void binsHasChanged(QString nbins);
    void overlapHasChanged(QString overlap);

private:
    void initControlWidget();
    struct {
      QComboBox * binsNuber;
      QComboBox * overlap;
      QComboBox * windowType;
    } m_baseControl;

    QWidget* createCombo(QString name, QComboBox *combo, QFont &font);
};

#endif // FREQPLOTWIDGETUI_H
