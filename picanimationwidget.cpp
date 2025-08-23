#include "picanimationwidget.h"
#include "protreeitem.h"
#include <QTimer>
#include <QPainter>

PicAnimationWidget::PicAnimationWidget(QWidget *parent)
    : QWidget{parent}, _factor(0.0), _cur_item(nullptr), _b_start(false), _b_paused(false)
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
    _b_paused = false; // 重置暂停状态
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
    
    // 缩放第一张图片，保持宽高比
    QPixmap scaledPixmap1 = _pixmap1.scaled(w, h, Qt::KeepAspectRatio);
    
    // 计算第一张图片的居中位置
    int x1 = (w - scaledPixmap1.width()) / 2;
    int y1 = (h - scaledPixmap1.height()) / 2;
    
    // 绘制第一张图片，设置透明度
    painter.setOpacity(1.0f - _factor); // 第一张图片透明度从1到0
    painter.drawPixmap(x1, y1, scaledPixmap1);
    
    // 如果第二张图片为空，直接返回
    if(_pixmap2.isNull())
    {
        return;
    }
    
    // 缩放第二张图片，保持宽高比
    QPixmap scaledPixmap2 = _pixmap2.scaled(w, h, Qt::KeepAspectRatio);
    
    // 计算第二张图片的居中位置
    int x2 = (w - scaledPixmap2.width()) / 2;
    int y2 = (h - scaledPixmap2.height()) / 2;
    
    // 绘制第二张图片，设置透明度
    painter.setOpacity(_factor); // 第二张图片透明度从0到1
    painter.drawPixmap(x2, y2, scaledPixmap2);
    
    // 重置透明度
    painter.setOpacity(1.0f);
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
        _map_items[path] = tree_item;
    }

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

    _factor = _factor + 0.01; //增加增量，让淡入淡出效果更明显
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
        if (_b_paused) {
            // 从暂停状态恢复，发送恢复音乐信号
            emit SigResumeMusic();
            _b_paused = false;
        } else {
            // 首次开始播放，发送开始音乐信号
            emit SigStartMusic();
        }
    }
    else
    {
        _timer->stop();
        _factor = 0;
        update();
        _b_start = false;
        _b_paused = true; // 标记为暂停状态
        emit SigPauseMusic(); //发送暂停音乐信号
    }
}
