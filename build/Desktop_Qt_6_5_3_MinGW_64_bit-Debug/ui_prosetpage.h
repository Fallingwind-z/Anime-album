/********************************************************************************
** Form generated from reading UI file 'prosetpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROSETPAGE_H
#define UI_PROSETPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_ProSetPage
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label_Name;
    QLabel *label_Path;
    QLineEdit *lineEdit_Path;
    QPushButton *pushButton_browse;
    QLineEdit *lineEdit_Name;
    QLabel *tips;

    void setupUi(QWizardPage *ProSetPage)
    {
        if (ProSetPage->objectName().isEmpty())
            ProSetPage->setObjectName("ProSetPage");
        ProSetPage->resize(1156, 721);
        gridLayout_2 = new QGridLayout(ProSetPage);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(5, 5, 5, 5);
        label_Name = new QLabel(ProSetPage);
        label_Name->setObjectName("label_Name");

        gridLayout->addWidget(label_Name, 0, 0, 1, 1);

        label_Path = new QLabel(ProSetPage);
        label_Path->setObjectName("label_Path");

        gridLayout->addWidget(label_Path, 1, 0, 1, 1);

        lineEdit_Path = new QLineEdit(ProSetPage);
        lineEdit_Path->setObjectName("lineEdit_Path");

        gridLayout->addWidget(lineEdit_Path, 1, 1, 1, 1);

        pushButton_browse = new QPushButton(ProSetPage);
        pushButton_browse->setObjectName("pushButton_browse");

        gridLayout->addWidget(pushButton_browse, 1, 2, 1, 1);

        lineEdit_Name = new QLineEdit(ProSetPage);
        lineEdit_Name->setObjectName("lineEdit_Name");

        gridLayout->addWidget(lineEdit_Name, 0, 1, 1, 1);

        tips = new QLabel(ProSetPage);
        tips->setObjectName("tips");

        gridLayout->addWidget(tips, 2, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(ProSetPage);

        QMetaObject::connectSlotsByName(ProSetPage);
    } // setupUi

    void retranslateUi(QWizardPage *ProSetPage)
    {
        ProSetPage->setWindowTitle(QCoreApplication::translate("ProSetPage", "WizardPage", nullptr));
        label_Name->setText(QCoreApplication::translate("ProSetPage", "Name:", nullptr));
        label_Path->setText(QCoreApplication::translate("ProSetPage", "Path:", nullptr));
        pushButton_browse->setText(QCoreApplication::translate("ProSetPage", "browse", nullptr));
        tips->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ProSetPage: public Ui_ProSetPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROSETPAGE_H
