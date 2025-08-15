#include "picshow.h"
#include "ui_picshow.h"

PicShow::PicShow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PicShow)
{
    ui->setupUi(this);

    ui->previousBtn->SetIcons(":/icon/previous.png", ":/icon/previous_hover.png", ":/icon/previous_press.png");
    ui->nextBtn->SetIcons(":/icon/next.png", ":/icon/next_hover.png", ":/icon/next_press.png");

    // 创建透明度效果对象，用于控制按钮的透明度
    opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0); //0.0 完全透明 → 1.0 完全不透明
    ui->previousBtn->setGraphicsEffect(opacity_pre);

    opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0); //0.0 完全透明 → 1.0 完全不透明
    ui->nextBtn->setGraphicsEffect(opacity_next);

    // 创建属性动画对象，控制按钮的透明度变化
    _animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this); // 参数：目标对象(opacity_pre)，属性名("opacity")，父对象(this)
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear); // 设置动画的缓动曲线为线性（匀速变化）
    _animation_show_pre->setDuration(500); // 设置动画持续时间为500毫秒

    _animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next->setDuration(500);

    connect(ui->previousBtn, &QPushButton::clicked, this, &PicShow::SigPreClicked); //按钮点击上一张通知PicShow发出信号，随后在MainWindow中连接信号槽
    connect(ui->nextBtn, &QPushButton::clicked, this, &PicShow::SigNextClicked); //按钮点击下一张通知PicShow发出信号，随后在MainWindow中连接信号槽
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::ReloadPic()
{
    if(_select_path != "")
    {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        _pix_map.load(_select_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->picLabel->setPixmap(_pix_map);
    }
}

bool PicShow::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Enter: ShowPreNextBtns(true); //当鼠标光标进入该控件区域时触发
        break;
    case QEvent::Leave: ShowPreNextBtns(false); //当鼠标光标离开该控件区域时触发
        break;
    Default:
        break;
    }
    return QDialog::event(e);
}

//根据传入的 b_show 决定是 显示 还是 隐藏 按钮
void PicShow::ShowPreNextBtns(bool b_show)
{
    if(!b_show && _b_btnvisible) //按钮可见，要求隐藏按钮
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1); //从完全显示
        _animation_show_pre->setEndValue(0); //变到完全隐藏
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1); //从完全显示
        _animation_show_next->setEndValue(0); //变到完全隐藏
        _animation_show_next->start();

        _b_btnvisible = false;

        return;
    }
    if(b_show && !_b_btnvisible) //按钮不可见，要求显示按钮
    {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0); //从完全显示
        _animation_show_pre->setEndValue(1); //变到完全隐藏
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0); //从完全显示
        _animation_show_next->setEndValue(1); //变到完全隐藏
        _animation_show_next->start();

        _b_btnvisible = true;

        return;
    }
}

void PicShow::SlotSelectItem(const QString &path)
{
    _select_path = path;
    _pix_map.load(path);
    auto width = this->width() - 20;
    auto height = this->height() - 20;
    _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
    ui->picLabel->setPixmap(_pix_map);
}

void PicShow::SlotUpdatePic(const QString &path)
{
    _select_path = path;
    if(_select_path != "")
    {
        const auto &width = ui->gridLayout->geometry().width();
        const auto &height = ui->gridLayout->geometry().height();
        _pix_map.load(_select_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->picLabel->setPixmap(_pix_map);
    }
}

void PicShow::SlotDeleteItem()
{
    _select_path = "";
}
