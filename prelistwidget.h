#ifndef PRELISTWIDGET_H
#define PRELISTWIDGET_H

#include <QListWidget>
#include <QTreeWidgetItem>

class PreListWidget : public QListWidget
{
    Q_OBJECT
public:
    PreListWidget(QWidget *parent = nullptr);
    virtual ~PreListWidget();

private:
    QMap<QString, QListWidgetItem *> _set_items;
    void AddListItem(const QString &path);
    int _global;
    QPoint _pos_origin;
    int _last_index;

public slots:
    void SlotUpdatePreList(QTreeWidgetItem *tree_item);
    void SlotSelectItem(QTreeWidgetItem *tree_item);
    void SlotItemPressed(QListWidgetItem *item);

signals:
    void SigUpdateSelectShow(QString path);
};

#endif // PRELISTWIDGET_H
