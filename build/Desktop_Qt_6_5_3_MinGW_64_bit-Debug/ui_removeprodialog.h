/********************************************************************************
** Form generated from reading UI file 'removeprodialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOVEPRODIALOG_H
#define UI_REMOVEPRODIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemoveProDialog
{
public:
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QLabel *label;
    QCheckBox *checkBox;
    QSpacerItem *verticalSpacer_3;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *RemoveProDialog)
    {
        if (RemoveProDialog->objectName().isEmpty())
            RemoveProDialog->setObjectName("RemoveProDialog");
        RemoveProDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(RemoveProDialog);
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        label = new QLabel(RemoveProDialog);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        checkBox = new QCheckBox(RemoveProDialog);
        checkBox->setObjectName("checkBox");

        verticalLayout->addWidget(checkBox);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        buttonBox = new QDialogButtonBox(RemoveProDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        retranslateUi(RemoveProDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, RemoveProDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, RemoveProDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(RemoveProDialog);
    } // setupUi

    void retranslateUi(QDialog *RemoveProDialog)
    {
        RemoveProDialog->setWindowTitle(QCoreApplication::translate("RemoveProDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("RemoveProDialog", "\344\273\216\351\241\271\347\233\256\344\270\255\345\210\240\351\231\244\346\226\207\344\273\266", nullptr));
        checkBox->setText(QCoreApplication::translate("RemoveProDialog", "\345\220\214\346\227\266\345\210\240\351\231\244\346\234\254\345\234\260\346\226\207\344\273\266\345\244\271\351\241\271\347\233\256\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoveProDialog: public Ui_RemoveProDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOVEPRODIALOG_H
