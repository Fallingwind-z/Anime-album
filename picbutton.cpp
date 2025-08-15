#include "picbutton.h"

PicButton::PicButton(QWidget *parent)
{

}

void PicButton::SetIcons(const QString &normal, const QString &hover, const QString &press)
{
    _normal = normal;
    _hover = hover;
    _press = press;

    QPixmap tempPixmap;
    tempPixmap.load(normal);
    this->resize(tempPixmap.size());
    this->setIcon(tempPixmap);
}

bool PicButton::event(QEvent *e)
{
    switch(e->type())
    {
        case QEvent::Enter: setHoverIcon(); //当鼠标光标进入该控件区域时触发
            break;
        case QEvent::Leave: setNormalIcon(); //当鼠标光标离开该控件区域时触发
            break;
        case QEvent::MouseButtonPress: setPressIcon(); //当鼠标按钮在控件上按下时触发
            break;
        case QEvent::MouseButtonRelease: setHoverIcon(); //当鼠标按钮在控件上释放时触发（通常跟在Press之后）
            break;
        Default:
            break;
    }
    return QPushButton::event(e);
}

void PicButton::setNormalIcon()
{
    QPixmap normalPixmap;
    normalPixmap.load(_normal);
    this->resize(normalPixmap.size());
    this->setIcon(normalPixmap);
}

void PicButton::setHoverIcon()
{
    QPixmap hoverPixmap;
    hoverPixmap.load(_hover);
    this->resize(hoverPixmap.size());
    this->setIcon(hoverPixmap);
}

void PicButton::setPressIcon()
{
    QPixmap pressPixmap;
    pressPixmap.load(_press);
    this->resize(pressPixmap.size());
    this->setIcon(pressPixmap);
}
