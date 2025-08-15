#include "protreewidget.h"
#include "protreeitem.h"
#include "protreethread.h"
#include "const.h"
#include "removeprodialog.h"
#include <QDir>
#include <QGuiApplication>
#include <QFileDialog>
#include <QMenu>

ProTreeWidget::ProTreeWidget(QWidget *parent):QTreeWidget(parent),
    _right_btn_item(nullptr), _active_item(nullptr), _import_progressdlg(nullptr), _open_progressdlg(nullptr),
    _selected_item(nullptr), _thread_create_pro(nullptr), _thread_open_pro(nullptr)
{
    this->setHeaderHidden(true);
    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    _action_import = new QAction(QIcon(":/icon/import.png"), tr("导入文件"), this);
    _action_setstart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"), this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"), this);
    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_action_setstart, &QAction::triggered, this, &ProTreeWidget::SlotSetActive);
    connect(_action_closepro, &QAction::triggered, this, &ProTreeWidget::SlotClosePro);
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotDoubleClickItem);
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

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *doubleitem, int col)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        auto *tree_doubleItem = dynamic_cast<ProTreeItem *>(doubleitem);
        if(!tree_doubleItem)
        {
            return;
        }

        int itemtype = tree_doubleItem->type();
        if(itemtype == TreeItemPic)
        {
            emit SigUpdateSelected(tree_doubleItem->GetPath());
            _selected_item = doubleitem;
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
    _import_progressdlg = new QProgressDialog(this); // 创建进度对话框

    //传递线程的参数是按照右值来构造的,使用std::ref用引用来传递
    /*
     * 当通过 std::thread构造函数传递参数时，参数会先被​​拷贝或移动​​到线程的独立内存空间中
     * 随后以​​右值​​（即临时对象或可移动的表达式）的形式传递给线程函数
     * 这一设计是为了确保线程的执行不依赖于主线程栈上的变量（避免悬空引用或指针）
    */
    //std::make_shared 是一个模板函数，用于动态分配内存并构造一个对象，同时返回一个指向该对象的 std::shared_ptr 智能指针
    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path), std::ref(path), _right_btn_item, file_count, this, nullptr);
    //.get() 方法用于从 std::shared_ptr 中提取 原始指针(ProTreeThread*)，以便用于信号槽连接
    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateProgress); //线程与进度对话框更新的信号槽连接
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishProgress); //线程与进度对话框完成的信号槽连接
    connect(_import_progressdlg, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelProgress); //进度对话框取消与ProTreeWidget的信号槽连接
    connect(this, &ProTreeWidget::SigCancelProgress, _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress); //进度对话框取消与线程的信号槽连接   
    _thread_create_pro->start();
    _import_progressdlg->setWindowTitle("Please wait...");
    _import_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _import_progressdlg->setRange(0, PROGRESS_WIDTH);
    _import_progressdlg->exec();
}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if(!_import_progressdlg) //判断进度对话框是否为空
    {
        return;
    }
    if(count >= PROGRESS_MAX)
    {
        _import_progressdlg->setValue(count%PROGRESS_MAX);
    }
    else
    {
        _import_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress()
{
    _import_progressdlg->setValue(PROGRESS_MAX);
    _import_progressdlg->deleteLater(); //下一次轮询时删除进度对话框
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress(); //取消信号,需要与线程链接起来
    delete _import_progressdlg;
    _import_progressdlg = nullptr;
}

void ProTreeWidget::SlotSetActive()
{
    if(!_right_btn_item) //判断点击条目是否有效
    {
        return;
    }

    QFont nullFont;
    nullFont.setBold(false);

    if(_active_item)
    {
        _active_item->setFont(0, nullFont); //将之前的活跃条目设置为普通字体
    }

    _active_item = _right_btn_item; //当前右键点击的条目(_right_btn_item)设为新的活跃条目
    nullFont.setBold(true);
    _active_item->setFont(0, nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if(res != QDialog::Accepted)
    {
        return;
    }

    bool b_remove = remove_pro_dialog.isRemoved(); //是否选择删除本地文件
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item); //获取当前条目得到索引
    auto *protreeitem = dynamic_cast<ProTreeItem *>(_right_btn_item); //当前右键的条目
    auto *selecteditem = dynamic_cast<ProTreeItem *>(_selected_item); //当前选择的条目
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);
    if(b_remove) //删除本地文件
    {
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if(protreeitem == _active_item)
    {
        _active_item = nullptr; //如果删除的是当前激活条目，将其置空
    }

    //关闭项目时要将显示的图片给清除
    if(selecteditem && protreeitem == selecteditem->GetRoot()) //selecteditem不为空并且protreeitem（右键的项目）正好是右侧显示图片的根节点
    {
        selecteditem = nullptr;
        _selected_item = nullptr;
        emit SigClearSelected();
    }

    delete this->takeTopLevelItem(index_right_btn); //将其从根目录树中移除
    _right_btn_item = nullptr;
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if(_set_path.find(path) != _set_path.end())
    {
        qDebug() << "file has loaded";
        return; //找到了说明打开了，已经在左侧目录树显示了，直接return
    }
    //如果没有将其加入目录树中
    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    QString proname = pro_dir.dirName(); //获取目录的名称
    _open_progressdlg = new QProgressDialog(this); // 创建打开进度对话框
    //std::make_shared 是一个模板函数，用于动态分配内存并构造一个对象，同时返回一个指向该对象的 std::shared_ptr 智能指针
    _thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count, this, nullptr);

    //连接打开进度框操作
    connect(_thread_open_pro.get(), &OpenTreeThread::SigUpdateProgress, this, &ProTreeWidget::SlotUpdateOpenProgress); //线程与进度对话框更新的信号槽连接
    connect(_thread_open_pro.get(), &OpenTreeThread::SigFinishProgress, this, &ProTreeWidget::SlotFinishOpenProgress); //线程与进度对话框完成的信号槽连接
    connect(_open_progressdlg, &QProgressDialog::canceled, this, &ProTreeWidget::SlotCancelOpenProgress); //进度对话框取消与ProTreeWidget的信号槽连接
    connect(this, &ProTreeWidget::SigCancelOpenProgress, _thread_open_pro.get(), &OpenTreeThread::SlotCancelOpenProgress); //进度对话框取消与线程的信号槽连接
    _thread_open_pro->start();
    _open_progressdlg->setWindowTitle("Please wait...");
    _open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progressdlg->setRange(0, PROGRESS_WIDTH);
    _open_progressdlg->exec();
}

void ProTreeWidget::SlotPreShow()
{
    if(!_selected_item)
    {
        return; //为空
    }

    auto *curItem = dynamic_cast<ProTreeItem *>(_selected_item)->GetPreItem(); // 调用GetPreItem()获取上一个item
    if(!curItem)
    {
        return;
    }

    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem); // 在UI树形控件中设置当前选中项（高亮显示）
}

void ProTreeWidget::SlotNextShow()
{
    if(!_selected_item)
    {
        return; //为空
    }

    auto *curItem = dynamic_cast<ProTreeItem *>(_selected_item)->GetNextItem(); // 调用GetNextItem()获取下一个item
    if(!curItem)
    {
        return;
    }

    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem); // 在UI树形控件中设置当前选中项（高亮显示）
}

void ProTreeWidget::SlotUpdateOpenProgress(int count)
{
    if(!_open_progressdlg) //判断进度对话框是否为空
    {
        return;
    }
    if(count >= PROGRESS_MAX)
    {
        _open_progressdlg->setValue(count%PROGRESS_MAX);
    }
    else
    {
        _open_progressdlg->setValue(count);
    }
}

void ProTreeWidget::SlotFinishOpenProgress()
{
    _open_progressdlg->setValue(PROGRESS_MAX);
    _open_progressdlg->deleteLater(); //下一次轮询时删除进度对话框
}

void ProTreeWidget::SlotCancelOpenProgress()
{
    emit SigCancelOpenProgress(); //取消信号,需要与线程链接起来
    delete _open_progressdlg;
    _open_progressdlg = nullptr;
}

