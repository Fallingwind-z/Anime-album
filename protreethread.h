#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidgetItem>

class ProTreeThread : public QThread
{
    Q_OBJECT //如果缺少 Q_OBJECT，信号和槽将无法正常连接和触发, 新加Q_OBJECT要执行qmake
public:
    ProTreeThread(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item,
                           int file_count, QTreeWidget *self, QTreeWidgetItem *root, QObject *parent = nullptr);
    ~ProTreeThread();

protected:
    virtual void run(); //一般情况下子类继承父类都是以protected继承

private:
    void CreateProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item,
                            int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *preItem = nullptr);

    QString _src_path;
    QString _dist_path;
    int _file_count;
    QTreeWidgetItem *_parent_item;
    QTreeWidget *_self;
    QTreeWidgetItem *_root;
    bool _bstop; //控制线程退出

public slots:
    void SlotCancelProgress();

signals: //signals 声明必须放在 Q_OBJECT 宏标记的类中
    void SigUpdateProgress(int);
    void SigFinishProgress(int);

};

#endif // PROTREETHREAD_H
