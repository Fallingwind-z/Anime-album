#include "protreeitem.h"

ProTreeItem::ProTreeItem(QTreeWidget *view, const QString &name, const QString &path, int type)
    :QTreeWidgetItem(view, type), _name(name), _path(path), _root(this), _pre_item(nullptr), _next_item(nullptr)
{

}

ProTreeItem::ProTreeItem(QTreeWidgetItem *parent, const QString &name, const QString &path, QTreeWidgetItem *root, int type)
    :QTreeWidgetItem(parent, type), _name(name), _path(path), _root(root), _pre_item(nullptr), _next_item(nullptr)
{

}

const QString &ProTreeItem::GetPath()
{
    return _path;
}

QTreeWidgetItem *ProTreeItem::GetRoot()
{
    return _root;
}

void ProTreeItem::SetPreItem(QTreeWidgetItem *item)
{
    _pre_item = item;
}

void ProTreeItem::SetNextItem(QTreeWidgetItem *item)
{
    _next_item = item;
}

ProTreeItem *ProTreeItem::GetPreItem()
{
    return dynamic_cast<ProTreeItem *>(_pre_item); //_pre_item是基类QTreeWidgetItem *类型，需要用安全转换dynamic_cast将其赋值给子类ProTreeItem *
}

ProTreeItem *ProTreeItem::GetNextItem()
{
    return dynamic_cast<ProTreeItem *>(_next_item); //_next_item是基类QTreeWidgetItem *类型，需要用安全转换dynamic_cast将其赋值给子类ProTreeItem *
}

ProTreeItem *ProTreeItem::GetLastPicChild()
{

}

ProTreeItem *ProTreeItem::GetFirstPicChild()
{

}
