/********************************************************************************
** Form generated from reading UI file 'selectremovecurvewindowdialog.ui'
**
** Created: Mon Oct 15 22:40:06 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTREMOVECURVEWINDOWDIALOG_H
#define UI_SELECTREMOVECURVEWINDOWDIALOG_H

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

class Ui_SelectRemoveCurveWindowDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QListView *listViewCurves;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButtonRemove;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButtonBack;

    void setupUi(QDialog *SelectRemoveCurveWindowDialog)
    {
        if (SelectRemoveCurveWindowDialog->objectName().isEmpty())
            SelectRemoveCurveWindowDialog->setObjectName(QString::fromUtf8("SelectRemoveCurveWindowDialog"));
        SelectRemoveCurveWindowDialog->resize(401, 340);
        verticalLayoutWidget = new QWidget(SelectRemoveCurveWindowDialog);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(0, -1, 401, 331));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(-1);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(5, 3, 5, 3);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        listViewCurves = new QListView(verticalLayoutWidget);
        listViewCurves->setObjectName(QString::fromUtf8("listViewCurves"));

        verticalLayout->addWidget(listViewCurves);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButtonRemove = new QPushButton(verticalLayoutWidget);
        pushButtonRemove->setObjectName(QString::fromUtf8("pushButtonRemove"));

        horizontalLayout->addWidget(pushButtonRemove);

        horizontalSpacer = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButtonBack = new QPushButton(verticalLayoutWidget);
        pushButtonBack->setObjectName(QString::fromUtf8("pushButtonBack"));

        horizontalLayout->addWidget(pushButtonBack);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(SelectRemoveCurveWindowDialog);
        QObject::connect(pushButtonRemove, SIGNAL(clicked()), SelectRemoveCurveWindowDialog, SLOT(removeButtonPressed()));
        QObject::connect(pushButtonBack, SIGNAL(clicked()), SelectRemoveCurveWindowDialog, SLOT(removeButtonPressed()));

        QMetaObject::connectSlotsByName(SelectRemoveCurveWindowDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectRemoveCurveWindowDialog)
    {
        SelectRemoveCurveWindowDialog->setWindowTitle(QApplication::translate("SelectRemoveCurveWindowDialog", "SelectRemoveCurveWindowDialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("SelectRemoveCurveWindowDialog", "Select curves to remove", 0, QApplication::UnicodeUTF8));
        pushButtonRemove->setText(QApplication::translate("SelectRemoveCurveWindowDialog", "Remove", 0, QApplication::UnicodeUTF8));
        pushButtonBack->setText(QApplication::translate("SelectRemoveCurveWindowDialog", "Go Back", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class SelectRemoveCurveWindowDialog: public Ui_SelectRemoveCurveWindowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTREMOVECURVEWINDOWDIALOG_H
