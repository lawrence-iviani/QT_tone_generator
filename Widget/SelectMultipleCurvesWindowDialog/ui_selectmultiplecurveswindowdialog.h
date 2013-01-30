/********************************************************************************
** Form generated from reading UI file 'selectmultiplecurveswindowdialog.ui'
**
** Created: Tue Jan 29 22:00:28 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTMULTIPLECURVESWINDOWDIALOG_H
#define UI_SELECTMULTIPLECURVESWINDOWDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelectMultipleCurvesWindowDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QListView *listViewCurves;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonOK;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonBack;

    void setupUi(QDialog *SelectMultipleCurvesWindowDialog)
    {
        if (SelectMultipleCurvesWindowDialog->objectName().isEmpty())
            SelectMultipleCurvesWindowDialog->setObjectName(QString::fromUtf8("SelectMultipleCurvesWindowDialog"));
        SelectMultipleCurvesWindowDialog->setWindowModality(Qt::WindowModal);
        SelectMultipleCurvesWindowDialog->resize(401, 340);
        verticalLayoutWidget = new QWidget(SelectMultipleCurvesWindowDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, -1, 401, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(-1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 3, 5, 3);
        titleLabel = new QLabel(verticalLayoutWidget);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        listViewCurves = new QListView(verticalLayoutWidget);
        listViewCurves->setObjectName(QString::fromUtf8("listViewCurves"));

        verticalLayout->addWidget(listViewCurves);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonOK = new QPushButton(verticalLayoutWidget);
        pushButtonOK->setObjectName(QString::fromUtf8("pushButtonOK"));

        horizontalLayout->addWidget(pushButtonOK);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonBack = new QPushButton(verticalLayoutWidget);
        pushButtonBack->setObjectName(QString::fromUtf8("pushButtonBack"));

        horizontalLayout->addWidget(pushButtonBack);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SelectMultipleCurvesWindowDialog);

        QMetaObject::connectSlotsByName(SelectMultipleCurvesWindowDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectMultipleCurvesWindowDialog)
    {
        SelectMultipleCurvesWindowDialog->setWindowTitle(QApplication::translate("SelectMultipleCurvesWindowDialog", "Select curves dialog", 0, QApplication::UnicodeUTF8));
        titleLabel->setText(QApplication::translate("SelectMultipleCurvesWindowDialog", "Select curves ........", 0, QApplication::UnicodeUTF8));
        pushButtonOK->setText(QApplication::translate("SelectMultipleCurvesWindowDialog", "Ok", 0, QApplication::UnicodeUTF8));
        pushButtonBack->setText(QApplication::translate("SelectMultipleCurvesWindowDialog", "Go Back", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectMultipleCurvesWindowDialog: public Ui_SelectMultipleCurvesWindowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTMULTIPLECURVESWINDOWDIALOG_H
