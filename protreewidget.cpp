#include "protreewidget.h"
#include "protreeitem.h"
#include "protreethread.h"
#include "const.h"
#include <QDir>
#include <QGuiApplication>
#include <QFileDialog>
#include <QMenu>

ProTreeWidget::ProTreeWidget(QWidget *parent)
{
    this->setHeaderHidden(true);
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    _action_import = new QAction(QIcon(":/icon/import.png"), tr("导入文件"), this);
    _action_setstart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"), this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"), this);
    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
}

void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name); //拼成路径
    if(_set_path.find(file_path) != _set_path.end()) // 检查路径集合中是否已存在该路径（防止重名）
    {
        return; //重名直接return
    }

    QDir pro_dir(file_path);
    if(!pro_dir.exists())
    {
        bool enable = pro_dir.mkpath(file_path);  // 如果不存在则创建目录
        if(!enable)
        {
            return; // 创建失败则返回
        }
    }

    _set_path.insert(file_path); // 将新路径添加到路径集合中
    auto *item = new ProTreeItem(this, name, file_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name); // 设置第一列显示的项目名称，Qt::DisplayRole：设置项的显示文本（字符串）
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png")); // 设置第一列的图标为目录图标，Qt::DecorationRole：设置项的图标
    item->setData(0, Qt::ToolTipRole, file_path); // 设置第一列的提示信息为完整路径，Qt::ToolTipRole：设置项的悬停提示信息
    this->addTopLevelItem(item); //将 item 添加到树形控件（ProTreeWidget）的最顶层，作为根节点显示
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton) //点击右键弹出菜单
    {
        QMenu menu(this);
        int itemtype = pressedItem->type(); // 获取当前点击的项的类型
        if(itemtype == TreeItemPro) //TreeItemPro = 1, 表示项目的条目
        {
            _right_btn_item = pressedItem; //记录当前右键点击的项
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos()); //在鼠标光标位置弹出菜单
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog; // 创建文件对话框对象
    file_dialog.setFileMode(QFileDialog::Directory); // 设置文件模式为选择目录
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    QString path = "";
    if(!_right_btn_item)
    {
        qDebug() << "_right_btn_item is empty";
        return;
    }
    path = dynamic_cast<ProTreeItem *>(_right_btn_item)->GetPath(); // 获取当前右键点击项的路径
    file_dialog.setDirectory(path); // 设置文件对话框的初始目录
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if(file_dialog.exec())
    {
        fileNames = file_dialog.selectedFiles(); // 获取用户选择的文件夹路径
    }
    if(fileNames.length() <= 0)
    {
        return; // 如果没有选择任何文件夹则返回
    }

    QString import_path = fileNames.at(0); // 获取第一个选择的文件夹路径
    int file_count = 0;
    _dialog_progress = new QProgressDialog(this); // 创建进度对话框

    //传递线程的参数是按照右值来构造的,使用std::ref用引用来传递
    /*
     * 当通过 std::thread构造函数传递参数时，参数会先被​​拷贝或移动​​到线程的独立内存空间中
     * 随后以​​右值​​（即临时对象或可移动的表达式）的形式传递给线程函数
     * 这一设计是为了确保线程的执行不依赖于主线程栈上的变量（避免悬空引用或指针）
    */
    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path), std::ref(path), _right_btn_item, file_count, this, nullptr);
    //.get() 方法用于从 std::shared_ptr 中提取 原始指针(ProTreeThread*)，以便用于信号槽连接
    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateProgress); //线程与进度对话框更新的信号槽连接
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishProgress); //线程与进度对话框完成的信号槽连接
    connect(_dialog_progress, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelProgress); //进度对话框取消与ProTreeWidget的信号槽连接
    connect(this, &ProTreeWidget::SigCancelProgress, _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress); //进度对话框取消与线程的信号槽连接

    _thread_create_pro->start();
    _dialog_progress->setWindowTitle("Please wait...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_WIDTH);
    _dialog_progress->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_dialog_progress) //判断进度对话框是否为空
    {
        return;
    }
    if(count >= PROGRESS_MAX)
    {
        _dialog_progress->setValue(count%PROGRESS_MAX);
    }
    else
    {
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress()
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater(); //下一次轮询时删除进度对话框
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress(); //取消信号,需要与线程链接起来
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

