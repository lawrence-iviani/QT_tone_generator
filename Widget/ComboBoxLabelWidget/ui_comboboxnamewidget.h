/********************************************************************************
** Form generated from reading UI file 'comboboxnamewidget.ui'
**
** Created: Sun Sep 16 20:00:02 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMBOBOXNAMEWIDGET_H
#define UI_COMBOBOXNAMEWIDGET_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ComboBoxNameWidget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *comboBox;

    void setupUi(QWidget *ComboBoxNameWidget)
    {
        if (ComboBoxNameWidget->objectName().isEmpty())
            ComboBoxNameWidget->setObjectName(QString::fromUtf8("ComboBoxNameWidget"));
        ComboBoxNameWidget->resize(209, 49);
        verticalLayoutWidget = new QWidget(ComboBoxNameWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(-1, -1, 211, 51));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout->addWidget(label);

        comboBox = new QComboBox(verticalLayoutWidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        verticalLayout->addWidget(comboBox);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);

        retranslateUi(ComboBoxNameWidget);

        QMetaObject::connectSlotsByName(ComboBoxNameWidget);
    } // setupUi

    void retranslateUi(QWidget *ComboBoxNameWidget)
    {
        ComboBoxNameWidget->setWindowTitle(QApplication::translate("ComboBoxNameWidget", "ComboBoxNameWidget", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ComboBoxNameWidget", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ComboBoxNameWidget: public Ui_ComboBoxNameWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMBOBOXNAMEWIDGET_H
