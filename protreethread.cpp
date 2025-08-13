#include "protreethread.h"
#include "protreeitem.h"
#include "const.h"
#include <QDir>

ProTreeThread::ProTreeThread(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item,
                             int file_count, QTreeWidget *self, QTreeWidgetItem *root, QObject *parent)
    :QThread(parent), _src_path(src_path), _dist_path(dist_path),
    _file_count(file_count), _parent_item(parent_item), _self(self), _root(root), _bstop(false)
{

}

void ProTreeThread::CreateProTree(const QString &src_path, const QString &dist_path, QTreeWidgetItem *parent_item,
                                  int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *preItem)
{
    if(_bstop)
    {
        return;
    }
    bool needcopy = true; //src_path和src_path是否需要拷贝
    if(src_path == dist_path)
    {
        needcopy = false;
    }

    //用src_path构造目录
    QDir import_dir(src_path);
    QStringList nameFilters; //设置文件过滤器
    import_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); //setFilter里写需要留下的
    import_dir.setSorting(QDir::Name);
    QFileInfoList list = import_dir.entryInfoList(); //存储过滤出来的有效文件夹
    for(int i = 0; i < list.size(); i++)
    {
        if(_bstop)
        {
            return;
        }

        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if(bIsDir) //是文件夹
        {
            //递归处理
            if(_bstop)
            {
                return;
            }

            file_count++;
            emit SigUpdateProgress(file_count);

            QDir dist_dir(_dist_path); //用目的路径构造目的文件夹
            QString sub_dist_path = dist_dir.absoluteFilePath(fileInfo.fileName()); //子文件夹路径
            QDir sub_dist_dir(sub_dist_path); //子文件夹路径转为目录
            if(!sub_dist_dir.exists()) // 子文件夹不存在
            {
                bool ok = sub_dist_dir.mkpath(sub_dist_path); // 递归创建目录路径
                if(!ok)
                {
                    continue; // 创建失败则跳过当前文件夹
                }
            }

            auto *item = new ProTreeItem(parent_item, fileInfo.fileName(), sub_dist_path, root, TreeItemDir);

            item->setData(0, Qt::DisplayRole, fileInfo.fileName()); // 第0列显示文件名
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png")); // 第0列显示图标
            item->setData(0, Qt::ToolTipRole, sub_dist_path); // 鼠标悬停时显示完整路径
            CreateProTree(fileInfo.absoluteFilePath(), sub_dist_path, item, file_count, self, root, preItem); //递归调用CreateProTree
        }
        else //不是文件夹，即文件
        {
            if(_bstop)
            {
                return;
            }

            const QString &suffix = fileInfo.completeSuffix(); //取出完整文件后缀
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg")
            {
                continue; //如果不是图片就跳过，不做src_path和dist_path的拷贝
            }

            file_count++; //是图片文件,文件数加1,方便进度对话框展示导入进度
            emit SigUpdateProgress(file_count);
            if(!needcopy)
            {
                continue;
            }
            //是图片，将图片拷贝到目的路径
            QDir dist_dir(dist_path); //目的文件夹
            QString dist_file_path = dist_dir.absoluteFilePath(fileInfo.fileName()); //将文件夹和要拷贝的图片文件拼成新的路径
            //fileInfo.absoluteFilePath()获取源文件的完整绝对路径
            if(!QFile::copy(fileInfo.absoluteFilePath(), dist_file_path)) //将图片文件复制到 dist_file_path 路径
            {
                continue;
            }
            // 创建一个新的树形项(ProTreeItem)来表示这个图片文件
            auto *item = new ProTreeItem(parent_item, fileInfo.fileName(), dist_file_path, root, TreeItemPic); //TreeItemPic = 3, 表示项目的图片
            item->setData(0, Qt::DisplayRole, fileInfo.fileName()); // 第0列显示文件名
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png")); // 第0列显示图标
            item->setData(0, Qt::ToolTipRole, dist_file_path); // 鼠标悬停时显示完整路径

            //双向链表
            if(preItem)
            {
                auto *pre_proitem = dynamic_cast<ProTreeItem *>(preItem);
                pre_proitem->SetNextItem(item);
            }

            item->SetPreItem(preItem);
            preItem = item;
        }
    }
}

//线程启动自动调用该函数
void ProTreeThread::run()
{
    CreateProTree(_src_path, _dist_path, _parent_item, _file_count, _self, _root);
    if(_bstop) //取消操作
    {
        auto path = dynamic_cast<ProTreeItem *>(_root)->GetPath(); // 获取根节点的路径
        auto index = _self->indexOfTopLevelItem(_root); // 获取根节点在顶层项的索引
        delete _self->takeTopLevelItem(index); // takeTopLevelItem从树控件中移除（但不删除）指定索引 index 的顶层项, delete才是从树控件中删除根节点
        QDir dir(path); // 创建QDir对象操作目录
        dir.removeRecursively(); // 递归删除整个目录
        return;
    }
    emit SigFinishProgress(_file_count);
}

ProTreeThread::~ProTreeThread()
{

}

void ProTreeThread::SlotCancelProgress()
{
    this->_bstop = true;
}

