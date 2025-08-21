#include "prelistwidget.h"
#include "protreeitem.h"
#include "const.h"
#include "prelistitem.h"
#include <QPainter>
#include <QGuiApplication>

PreListWidget::PreListWidget(QWidget *parent):_global(0), QListWidget(parent), _last_index(17)
{
    this->setViewMode(QListWidget::IconMode); //图标模式
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);
    connect(this, &PreListWidget::itemPressed, this, &PreListWidget::SlotItemPressed);
}

PreListWidget::~PreListWidget()
{

}

void PreListWidget::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dst_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dst_pixmap.fill(QColor(220, 220, 220, 50));
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();
    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();
    QPainter painter(&dst_pixmap);
    auto x = (dst_width - src_width) / 2;
    auto y = (dst_height - src_height) / 2;
    painter.drawPixmap(x, y, src_pixmap);
    _global++;

    PreListItem *pItem = new PreListItem(QIcon(dst_pixmap), path, _global, this);
    pItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
    this->addItem(pItem);
    _set_items[path] = pItem;
    if(_global == 1)
    {
        _pos_origin = this->pos();
    }
}

void PreListWidget::SlotUpdatePreList(QTreeWidgetItem *tree_item)
{
    if(!tree_item)
    {
        return;
    }

    auto *pro_item = dynamic_cast<ProTreeItem *>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if(iter != _set_items.end())
    {
        return; //存在了
    }
    AddListItem(path);
}

void PreListWidget::SlotSelectItem(QTreeWidgetItem *tree_item)
{
    if(!tree_item)
    {
        return;
    }

    auto *pro_item = dynamic_cast<ProTreeItem *>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if(iter == _set_items.end())
    {
        return;
    }

    auto *list_item = dynamic_cast<PreListItem *>(iter.value());
    auto index = list_item->GetIndex();
    if(index > 17) //大于17实现向左滑动的效果
    {
        auto pos_cur = this->pos();
        this->move(pos_cur.x() - (index - _last_index) * 100, pos_cur.y());
    }
    else
    {
        this->move(_pos_origin);
        _last_index = 17;
    }

    this->setCurrentItem(iter.value());
}

void PreListWidget::SlotItemPressed(QListWidgetItem *item)
{
    if(QGuiApplication::mouseButtons() != Qt::LeftButton)
    {
        return;
    }

    auto *list_item = dynamic_cast<PreListItem *>(item);
    auto cur_index = list_item->GetIndex();
    auto path = list_item->GetPath();
    this->setCurrentItem(item);
    emit SigUpdateSelectShow(path);
}
