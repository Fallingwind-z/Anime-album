#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "protreethread.h"
#include "opentreethread.h"

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString &name, const QString &path);

private:
    QSet<QString> _set_path; //用来判断重名
    QTreeWidgetItem *_right_btn_item; //右键单击某个item之后弹出菜单
    QTreeWidgetItem *_active_item; //存储当前激活条目
    QTreeWidgetItem *_selected_item; //当前选中的条目
    QAction *_action_import;
    QAction *_action_setstart;
    QAction *_action_closepro;
    QAction *_action_slideshow;
    QProgressDialog *_import_progressdlg;
    QProgressDialog *_open_progressdlg;
    std::shared_ptr<ProTreeThread> _thread_create_pro; //用智能指针管理创建项目线程的生命周期
    std::shared_ptr<OpenTreeThread> _thread_open_pro; //用智能指针管理打开项目线程的生命周期

private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotDoubleClickItem(QTreeWidgetItem *doubleitem, int col);
    void SlotImport();
    void SlotSetActive();
    void SlotClosePro();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotUpdateOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelOpenProgress();

public slots:
    void SlotOpenPro(const QString &path); //需要用public slots，因为是从mainwindow中来的
    void SlotPreShow();
    void SlotNextShow();

signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString &path);
    void SigUpdatePic(const QString &path);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
