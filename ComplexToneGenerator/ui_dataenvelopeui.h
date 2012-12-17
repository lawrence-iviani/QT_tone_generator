/********************************************************************************
** Form generated from reading UI file 'dataenvelopeui.ui'
**
** Created: Sun Dec 16 12:55:05 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DATAENVELOPEUI_H
#define UI_DATAENVELOPEUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DataEnvelopeUI
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBoxEnableEnvelope;
    QWidget *widgetAmplitude;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *hlayoutAmplitude;
    QWidget *widgetTime;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *hlayoutTime;

    void setupUi(QWidget *DataEnvelopeUI)
    {
        if (DataEnvelopeUI->objectName().isEmpty())
            DataEnvelopeUI->setObjectName(QString::fromUtf8("DataEnvelopeUI"));
        DataEnvelopeUI->resize(253, 331);
        verticalLayoutWidget = new QWidget(DataEnvelopeUI);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, 0, 251, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        checkBoxEnableEnvelope = new QCheckBox(verticalLayoutWidget);
        checkBoxEnableEnvelope->setObjectName(QString::fromUtf8("checkBoxEnableEnvelope"));

        verticalLayout->addWidget(checkBoxEnableEnvelope);

        widgetAmplitude = new QWidget(verticalLayoutWidget);
        widgetAmplitude->setObjectName(QString::fromUtf8("widgetAmplitude"));
        horizontalLayoutWidget_2 = new QWidget(widgetAmplitude);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 251, 161));
        hlayoutAmplitude = new QHBoxLayout(horizontalLayoutWidget_2);
        hlayoutAmplitude->setObjectName(QString::fromUtf8("hlayoutAmplitude"));
        hlayoutAmplitude->setSizeConstraint(QLayout::SetMinimumSize);
        hlayoutAmplitude->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(widgetAmplitude);

        widgetTime = new QWidget(verticalLayoutWidget);
        widgetTime->setObjectName(QString::fromUtf8("widgetTime"));
        horizontalLayoutWidget = new QWidget(widgetTime);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 0, 251, 161));
        hlayoutTime = new QHBoxLayout(horizontalLayoutWidget);
        hlayoutTime->setObjectName(QString::fromUtf8("hlayoutTime"));
        hlayoutTime->setSizeConstraint(QLayout::SetMinimumSize);
        hlayoutTime->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(widgetTime);


        retranslateUi(DataEnvelopeUI);

        QMetaObject::connectSlotsByName(DataEnvelopeUI);
    } // setupUi

    void retranslateUi(QWidget *DataEnvelopeUI)
    {
        DataEnvelopeUI->setWindowTitle(QApplication::translate("DataEnvelopeUI", "Form", 0, QApplication::UnicodeUTF8));
        checkBoxEnableEnvelope->setText(QApplication::translate("DataEnvelopeUI", "Enable envelope", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DataEnvelopeUI: public Ui_DataEnvelopeUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DATAENVELOPEUI_H
