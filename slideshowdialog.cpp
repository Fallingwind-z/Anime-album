#include "slideshowdialog.h"
#include "ui_slideshowdialog.h"
#include "prelistwidget.h"
#include "protreewidget.h"

SlideShowDialog::SlideShowDialog(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item)
    : QDialog(parent), _first_item(first_item), _last_item(last_item)
    , ui(new Ui::SlideShowDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); //隐藏幻灯片放映的窗口和图标
    ui->slidepreBtn->SetIcons(":/icon/previous.png", ":/icon/previous_hover.png", ":/icon/previous_press.png");
    ui->slidenextBtn->SetIcons(":/icon/next.png", ":/icon/next_hover.png", ":/icon/next_press.png");
    ui->playBtn->SetIcons(":/icon/play.png", ":/icon/play_hover.png", ":/icon/play_press.png",
                          ":/icon/pause.png", ":/icon/pause_hover.png", ":/icon/pause_press.png");
    ui->closeBtn->SetIcons(":/icon/closeshow.png", ":/icon/closeshow_hover.png", ":/icon/closeshow_press.png");
    connect(ui->closeBtn, &QPushButton::clicked, this, &SlideShowDialog::close);
    connect(ui->slidepreBtn, &QPushButton::clicked, this, &SlideShowDialog::SlotSlidePre);
    connect(ui->slidenextBtn, &QPushButton::clicked, this, &SlideShowDialog::SlotSlideNext);

    auto *prelistWid = dynamic_cast<PreListWidget *>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWidget::SigUpdatePreList, prelistWid, &PreListWidget::SlotUpdatePreList); //动画窗口信号传递给预览窗口
    connect(ui->picAnimation, &PicAnimationWidget::SigSelectItem, prelistWid, &PreListWidget::SlotSelectItem);
    connect(prelistWid, &PreListWidget::SigUpdateSelectShow, ui->picAnimation, &PicAnimationWidget::SlotUpdateSelectShow);
    connect(ui->playBtn, &PicStatusButton::clicked, ui->picAnimation, &PicAnimationWidget::SlotStartOrStop);
    connect(ui->picAnimation, &PicAnimationWidget::SigStart, ui->playBtn, &PicStatusButton::SlotStart);
    connect(ui->picAnimation, &PicAnimationWidget::SigStop, ui->playBtn, &PicStatusButton::SlotStop);

    auto *_protree_widget = dynamic_cast<ProTreeWidget *>(parent);
    connect(ui->picAnimation, &PicAnimationWidget::SigStartMusic, _protree_widget, &ProTreeWidget::SlotStartMusic);
    connect(ui->picAnimation, &PicAnimationWidget::SigStopMusic, _protree_widget, &ProTreeWidget::SlotStopMusic);
    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();
}

SlideShowDialog::~SlideShowDialog()
{
    delete ui;
}

void SlideShowDialog::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}

void SlideShowDialog::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}
