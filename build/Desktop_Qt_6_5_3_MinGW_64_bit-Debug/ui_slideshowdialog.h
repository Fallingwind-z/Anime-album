/********************************************************************************
** Form generated from reading UI file 'slideshowdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLIDESHOWDIALOG_H
#define UI_SLIDESHOWDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <picanimationwidget.h>
#include <picbutton.h>
#include <picstatusbutton.h>
#include <prelistwidget.h>

QT_BEGIN_NAMESPACE

class Ui_SlideShowDialog
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *slideShow;
    QGridLayout *gridLayout;
    PicAnimationWidget *picAnimation;
    QWidget *slidePrewid;
    QVBoxLayout *verticalLayout_2;
    PicButton *slidepreBtn;
    QWidget *slideNextwid;
    QVBoxLayout *verticalLayout_3;
    PicButton *slidenextBtn;
    QHBoxLayout *horizontalLayout_3;
    PicStatusButton *playBtn;
    PicButton *closeBtn;
    QWidget *preShow;
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    PreListWidget *preListWidget;

    void setupUi(QDialog *SlideShowDialog)
    {
        if (SlideShowDialog->objectName().isEmpty())
            SlideShowDialog->setObjectName("SlideShowDialog");
        SlideShowDialog->resize(638, 522);
        verticalLayout = new QVBoxLayout(SlideShowDialog);
        verticalLayout->setObjectName("verticalLayout");
        slideShow = new QWidget(SlideShowDialog);
        slideShow->setObjectName("slideShow");
        gridLayout = new QGridLayout(slideShow);
        gridLayout->setObjectName("gridLayout");
        picAnimation = new PicAnimationWidget(slideShow);
        picAnimation->setObjectName("picAnimation");

        gridLayout->addWidget(picAnimation, 1, 1, 1, 1);

        slidePrewid = new QWidget(slideShow);
        slidePrewid->setObjectName("slidePrewid");
        slidePrewid->setMinimumSize(QSize(80, 0));
        slidePrewid->setMaximumSize(QSize(80, 16777215));
        verticalLayout_2 = new QVBoxLayout(slidePrewid);
        verticalLayout_2->setObjectName("verticalLayout_2");
        slidepreBtn = new PicButton(slidePrewid);
        slidepreBtn->setObjectName("slidepreBtn");

        verticalLayout_2->addWidget(slidepreBtn);


        gridLayout->addWidget(slidePrewid, 1, 0, 1, 1);

        slideNextwid = new QWidget(slideShow);
        slideNextwid->setObjectName("slideNextwid");
        slideNextwid->setMinimumSize(QSize(80, 0));
        slideNextwid->setMaximumSize(QSize(80, 16777215));
        verticalLayout_3 = new QVBoxLayout(slideNextwid);
        verticalLayout_3->setObjectName("verticalLayout_3");
        slidenextBtn = new PicButton(slideNextwid);
        slidenextBtn->setObjectName("slidenextBtn");

        verticalLayout_3->addWidget(slidenextBtn);


        gridLayout->addWidget(slideNextwid, 1, 2, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        playBtn = new PicStatusButton(slideShow);
        playBtn->setObjectName("playBtn");

        horizontalLayout_3->addWidget(playBtn);

        closeBtn = new PicButton(slideShow);
        closeBtn->setObjectName("closeBtn");

        horizontalLayout_3->addWidget(closeBtn);


        gridLayout->addLayout(horizontalLayout_3, 0, 2, 1, 1);


        verticalLayout->addWidget(slideShow);

        preShow = new QWidget(SlideShowDialog);
        preShow->setObjectName("preShow");
        horizontalLayout = new QHBoxLayout(preShow);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(preShow);
        widget->setObjectName("widget");
        widget->setMinimumSize(QSize(0, 120));
        widget->setMaximumSize(QSize(16777215, 120));
        preListWidget = new PreListWidget(widget);
        preListWidget->setObjectName("preListWidget");
        preListWidget->setGeometry(QRect(10, 10, 16777215, 110));
        preListWidget->setMinimumSize(QSize(0, 110));
        preListWidget->setMaximumSize(QSize(16777215, 110));
        preListWidget->setFlow(QListView::LeftToRight);
        preListWidget->setViewMode(QListView::IconMode);

        horizontalLayout->addWidget(widget);


        verticalLayout->addWidget(preShow);

        verticalLayout->setStretch(0, 7);
        verticalLayout->setStretch(1, 1);

        retranslateUi(SlideShowDialog);

        QMetaObject::connectSlotsByName(SlideShowDialog);
    } // setupUi

    void retranslateUi(QDialog *SlideShowDialog)
    {
        SlideShowDialog->setWindowTitle(QCoreApplication::translate("SlideShowDialog", "Dialog", nullptr));
        slidepreBtn->setText(QString());
        slidenextBtn->setText(QString());
        playBtn->setText(QString());
        closeBtn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class SlideShowDialog: public Ui_SlideShowDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLIDESHOWDIALOG_H
