/********************************************************************************
** Form generated from reading UI file 'selectcurvewindowdialog.ui'
**
** Created: Tue Jan 29 22:00:25 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTCURVEWINDOWDIALOG_H
#define UI_SELECTCURVEWINDOWDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectCurveWindowDialog
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *verticalSpacer_5;
    QComboBox *comboBox;
    QSpacerItem *verticalSpacer_4;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pushButton_2;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *SelectCurveWindowDialog)
    {
        if (SelectCurveWindowDialog->objectName().isEmpty())
            SelectCurveWindowDialog->setObjectName(QString::fromUtf8("SelectCurveWindowDialog"));
        SelectCurveWindowDialog->setWindowModality(Qt::ApplicationModal);
        SelectCurveWindowDialog->resize(387, 110);
        widget = new QWidget(SelectCurveWindowDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(0, 0, 388, 111));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        verticalSpacer_5 = new QSpacerItem(60, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_2->addItem(verticalSpacer_5);

        comboBox = new QComboBox(widget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);
        comboBox->setMinimumSize(QSize(250, 0));
        comboBox->setBaseSize(QSize(1000, 0));

        horizontalLayout_2->addWidget(comboBox);

        verticalSpacer_4 = new QSpacerItem(60, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout_2->addItem(verticalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(pushButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer_2);

        pushButton_2 = new QPushButton(widget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(120, 0));

        horizontalLayout->addWidget(pushButton_2);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        horizontalLayout->addItem(verticalSpacer_3);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SelectCurveWindowDialog);
        QObject::connect(pushButton, SIGNAL(clicked()), SelectCurveWindowDialog, SLOT(accept()));
        QObject::connect(pushButton_2, SIGNAL(clicked()), SelectCurveWindowDialog, SLOT(cancelSelect()));

        QMetaObject::connectSlotsByName(SelectCurveWindowDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectCurveWindowDialog)
    {
        SelectCurveWindowDialog->setWindowTitle(QApplication::translate("SelectCurveWindowDialog", "SelectCurveWindowDialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SelectCurveWindowDialog", "No Curve description", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("SelectCurveWindowDialog", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("SelectCurveWindowDialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectCurveWindowDialog: public Ui_SelectCurveWindowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTCURVEWINDOWDIALOG_H
