/********************************************************************************
** Form generated from reading UI file 'confirmpage.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIRMPAGE_H
#define UI_CONFIRMPAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_ConfirmPage
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;

    void setupUi(QWizardPage *ConfirmPage)
    {
        if (ConfirmPage->objectName().isEmpty())
            ConfirmPage->setObjectName("ConfirmPage");
        ConfirmPage->resize(950, 669);
        gridLayout_2 = new QGridLayout(ConfirmPage);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(5, 5, 5, 5);
        label = new QLabel(ConfirmPage);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(10);
        label->setFont(font);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);


        retranslateUi(ConfirmPage);

        QMetaObject::connectSlotsByName(ConfirmPage);
    } // setupUi

    void retranslateUi(QWizardPage *ConfirmPage)
    {
        ConfirmPage->setWindowTitle(QCoreApplication::translate("ConfirmPage", "WizardPage", nullptr));
        label->setText(QCoreApplication::translate("ConfirmPage", "\347\202\271\345\207\273\345\256\214\346\210\220\357\274\214\345\256\214\346\210\220\345\210\233\345\273\272\357\274\201\346\202\250\344\271\237\345\217\257\344\273\245\347\202\271\345\207\273\345\217\226\346\266\210\357\274\214\345\217\226\346\266\210\346\234\254\346\254\241\345\210\233\345\273\272\343\200\202", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConfirmPage: public Ui_ConfirmPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIRMPAGE_H
