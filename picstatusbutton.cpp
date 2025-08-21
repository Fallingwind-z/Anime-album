#include "picstatusbutton.h"
#include "const.h"
#include <QEvent>

PicStatusButton::PicStatusButton(QWidget *parent):QPushButton(parent)
{

}

void PicStatusButton::SetIcons(const QString &play_normal, const QString &play_hover, const QString &play_press,
                               const QString &pause_normal, const QString &pause_hover, const QString &pause_press)
{
    _play_normal = play_normal;
    _play_hover = play_hover;
    _play_press = play_press;
    _pause_normal = pause_normal;
    _pause_hover = pause_hover;
    _pause_press = pause_press;

    QPixmap tempPixmap;
    tempPixmap.load(play_normal);
    this->resize(tempPixmap.size());
    this->setIcon(tempPixmap);
    this->setIconSize(tempPixmap.size());
    _cur_state = PicButtonStatePlayNormal;
}

PicStatusButton::~PicStatusButton()
{

}

bool PicStatusButton::event(QEvent *e)
{
    switch (e->type())
    {
        case QEvent::Enter:
            if(_cur_state < PicButtonStatePauseNormal) //显示的是播放图标
            {
                setPlayHoverIcon();
            }
            else
            {
                setPauseHoverIcon();
            }
            break;
        case QEvent::Leave:
            if(_cur_state < PicButtonStatePauseNormal) //显示的是播放图标
            {
                setPlayNormalIcon();
            }
            else
            {
                setPauseNormalIcon();
            }
            break;
        case QEvent::MouseButtonPress:
            if(_cur_state < PicButtonStatePauseNormal) //显示的是播放图标
            {
                setPlayPressIcon();
            }
            else
            {
                setPausePressIcon();
            }
            break;
        case QEvent::MouseButtonRelease:
            if(_cur_state < PicButtonStatePauseNormal) //显示的是播放图标
            {
                setPauseHoverIcon();
            }
            else
            {
                setPlayHoverIcon();
            }
            break;
        default:
            break;
    }
    return QPushButton::event(e);
}

void PicStatusButton::setPlayNormalIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_play_normal);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePlayNormal;
}

void PicStatusButton::setPlayHoverIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_play_hover);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePlayHover;
}

void PicStatusButton::setPlayPressIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_play_press);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePlayPress;
}

void PicStatusButton::setPauseNormalIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_pause_normal);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePauseNormal;
}

void PicStatusButton::setPauseHoverIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_pause_hover);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePauseHover;
}

void PicStatusButton::setPausePressIcon()
{
    QPixmap tempPixmap;
    tempPixmap.load(_pause_press);
    this->setIcon(tempPixmap);
    _cur_state = PicButtonStatePausePress;
}

void PicStatusButton::SlotStart()
{
    setPauseNormalIcon();
}

void PicStatusButton::SlotStop()
{
    setPlayNormalIcon();
}
