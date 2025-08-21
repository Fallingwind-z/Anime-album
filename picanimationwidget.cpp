#include "picanimationwidget.h"
#include "protreeitem.h"
#include <QTimer>
#include <QPainter>

PicAnimationWidget::PicAnimationWidget(QWidget *parent)
    : QWidget{parent}, _factor(0.0), _cur_item(nullptr), _b_start(false)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &PicAnimationWidget::TimeOut);
}

PicAnimationWidget::~PicAnimationWidget()
{

}

void PicAnimationWidget::SetPixmap(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }

    auto *tree_item = dynamic_cast<ProTreeItem *>(item);
    auto path = tree_item->GetPath();
    _pixmap1.load(path);
    _cur_item = tree_item;

    if(_map_items.find(path) == _map_items.end()) //没有找到
    {
        _map_items[path] = tree_item;
        //发送更新预览列表逻辑
        emit SigUpdatePreList(item);
    }
    emit SigSelectItem(item);

    auto *next_item = tree_item->GetNextItem();
    if(!next_item)
    {
        return;
    }

    auto next_path = next_item->GetPath();
    _pixmap2.load(next_path);
    if(_map_items.find(next_path) == _map_items.end())
    {
        _map_items[next_path] = next_item;
    }
    //发送更新预览列表逻辑
    emit SigUpdatePreList(next_item);
}

void PicAnimationWidget::Start()
{
    emit SigStart();
    emit SigStartMusic();
    _factor = 0;
    _timer->start(25);
    _b_start = true;
}

void PicAnimationWidget::Stop()
{
    emit SigStop();
    emit SigStopMusic();
    _timer->stop();
    _factor = 0;
    _b_start = false;
}

void PicAnimationWidget::SlidePre()
{
    Stop();
    if(!_cur_item)
    {
        return;
    }

    auto *cur_pro_item = dynamic_cast<ProTreeItem *>(_cur_item);
    auto *pre_item = cur_pro_item->GetPreItem();
    if(!pre_item)
    {
        return;
    }
    SetPixmap(pre_item);
    update();
}

void PicAnimationWidget::SlideNext()
{
    Stop();
    if(!_cur_item)
    {
        return;
    }

    auto *cur_pro_item = dynamic_cast<ProTreeItem *>(_cur_item);
    auto *next_item = cur_pro_item->GetNextItem();
    if(!next_item)
    {
        return;
    }
    SetPixmap(next_item);
    update();
}

//Qt绘图事件处理函数，用于实现两张图片之间的淡入淡出动画效果
void PicAnimationWidget::paintEvent(QPaintEvent *event)
{
    // 如果第一张图片为空，直接返回
    if(_pixmap1.isNull())
    {
        return;
    }
    QPainter painter(this); // painter用来管理p1和p2
    painter.setRenderHint(QPainter::Antialiasing, true); //设置抗锯齿效果
    QRect rect = geometry(); // 获取当前控件的大小
    int w = rect.width();
    int h = rect.height();
    _pixmap1 = _pixmap1.scaled(w, h, Qt::KeepAspectRatio); // 等比拉伸第一张图片
    int alpha = 255 * (1.0f - _factor); // 计算第一张图片的透明度（根据_factor因子，范围0-1）
    QPixmap alphaPixmap1(_pixmap1.size()); // 创建临时图片
    alphaPixmap1.fill(Qt::transparent); // 填充成透明背景，相当于_pixmap1的遮罩

    QPainter p1(&alphaPixmap1); //p1用来画alphaPixmap
    p1.setCompositionMode(QPainter::CompositionMode_Source); // CompositionMode_Source直接写入新像素，忽略目标位置原有内容
    p1.drawPixmap(0, 0, _pixmap1); // 在临时图片alphaPixmap的(0,0)位置绘制原始图片(_pixmap1)
    p1.setCompositionMode(QPainter::CompositionMode_Destination); // CompositionMode_Destination只修改目标位置像素的alpha值，保持颜色不变
    p1.fillRect(alphaPixmap1.rect(), QColor(0, 0, 0, alpha));
    p1.end(); // 结束绘制操作，确保所有绘图指令执行完成
    // 计算居中位置并绘制第一张图片
    int x = (w - _pixmap1.width()) / 2;
    int y = (h - _pixmap1.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap1);

    // 如果第二张图片为空，直接返回
    if(_pixmap2.isNull())
    {
        return;
    }
    // 缩放第二张图片，保持宽高比
    _pixmap2 = _pixmap2.scaled(w, h, Qt::KeepAspectRatio);
    alpha = 255 * (_factor); // 计算第二张图片的透明度（与第一张相反）
    QPixmap alphaPixmap2(_pixmap2.size()); // 创建透明背景的临时图片
    alphaPixmap2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap2); // 在临时图片上绘制第二张图片并设置透明度
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, _pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_Destination);
    p2.fillRect(alphaPixmap2.rect(), QColor(0, 0, 0, alpha));
    p2.end();
    // 计算居中位置并绘制第二张图片
    x = (w - _pixmap2.width()) / 2;
    y = (h - _pixmap2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap2);
}

void PicAnimationWidget::UpdateSelectPixmap(QTreeWidgetItem *item)
{
    if(!item)
    {
        return;
    }

    auto *tree_item = dynamic_cast<ProTreeItem *>(item);
    auto path = tree_item->GetPath();
    _pixmap1.load(path);
    _cur_item = tree_item;

    if(_map_items.find(path) == _map_items.end())
    {
        _map_items[path] == tree_item;
    }

    auto *next_item = tree_item->GetNextItem();
    if(!next_item)
    {
        return;
    }

    auto next_path = tree_item->GetPath();
    _pixmap2.load(next_path);
    if(_map_items.find(next_path) == _map_items.end())
    {
        _map_items[next_path] = next_item;
    }
}

void PicAnimationWidget::TimeOut()
{
    //判断状态
    if(!_cur_item) //_cur_item为空
    {
        Stop(); //停止动画
        update(); //刷新页面,会自动调用paintEvent
        return;
    }

    _factor = _factor + 0.01;
    if(_factor >= 1) //_factor >= 1 播放下一张图片
    {
        _factor = 0;
        auto *cur_pro_item = dynamic_cast<ProTreeItem *>(_cur_item);
        auto *next_pro_item = cur_pro_item->GetNextItem();
        if(!next_pro_item)
        {
            Stop();
            update(); //刷新页面
            return;
        }

        SetPixmap(next_pro_item);
        update();
        return;
    }

    update();
}

void PicAnimationWidget::SlotUpdateSelectShow(QString path)
{
    auto iter = _map_items.find(path);
    if(iter == _map_items.end())
    {
        return;
    }
    UpdateSelectPixmap(iter.value());
    update();
}

void PicAnimationWidget::SlotStartOrStop()
{
    if(!_b_start)
    {
        _factor = 0;
        _timer->start(25);
        _b_start = true;
        emit SigStartMusic();
    }
    else
    {
        _timer->stop();
        _factor = 0;
        update();
        _b_start = false;
        emit SigStopMusic();
    }
}
