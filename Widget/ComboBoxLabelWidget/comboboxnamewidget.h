#ifndef COMBOBOXNAMEWIDGET_H
#define COMBOBOXNAMEWIDGET_H

#include <QWidget>
#include <QDebug>
namespace Ui {
class ComboBoxNameWidget;
}

class ComboBoxNameWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ComboBoxNameWidget(QWidget *parent = 0);
    ~ComboBoxNameWidget();

    void addItem(const QString &item);
    void addItems(const QStringList &items);
    void removeItem(int index);
    void setFont(QFont  f);
    void setText(const QString &text);

    QString item();
    int index();
    QString text();

public slots:
    void setIndex(int index);
signals:
    void currentIndexChanged(int index);
private:
    Ui::ComboBoxNameWidget *ui;
};

#endif // COMBOBOXNAMEWIDGET_H
