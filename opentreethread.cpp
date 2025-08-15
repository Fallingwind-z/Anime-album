#include "opentreethread.h"
#include "protreeitem.h"
#include "const.h"
#include <QDir>

OpenTreeThread::OpenTreeThread(const QString &src_path, int file_count, QTreeWidget *self, QObject *parent)
    :QThread(parent), _src_path(src_path), _file_count(file_count), _self(self), _bstop(false), _root(nullptr)
{

}

void OpenTreeThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    //创建根节点
    QDir src_dir(src_path);
    auto name = src_dir.dirName(); //返回目录路径的最后一级名称（即最右侧的部分）
    auto *item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, src_path);
    _root = item; // 将当前创建的项设置为根项

    //读取根节点下目录和文件
    RecursiveProTree(src_path, file_count, self, _root, item, nullptr);
}

void OpenTreeThread::run()
{
    OpenProTree(_src_path, _file_count, _self);
    //取消操作
    if(_bstop)
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

void OpenTreeThread::RecursiveProTree(const QString &src_path, int file_count, QTreeWidget *self,
                                      QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preItem)
{
    QDir src_dir(src_path);
    /*
     * QDir::Dirs - 包含子目录
     * QDir::Files - 包含文件
     * QDir::NoDotAndDotDot - 不包含特殊目录"."(当前目录)和".."(上级目录)
    */
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot); //过滤器
    src_dir.setSorting(QDir::DirsFirst | QDir::Name); //QDir::DirsFirst目录优先、QDir::Name表示按名称排序(字母顺序)
    QFileInfoList list = src_dir.entryInfoList(); //获取满足过滤条件的目录内容列表，其中每个QFileInfo对象包含一个文件或目录的详细信息(如名称、大小、修改时间等)
    for(int i = 0; i < list.size(); i++)
    {
        if(_bstop)
        {
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir(); //判断是否是目录
        if(bIsDir) //如果是目录则递归调用
        {
            if(_bstop)
            {
                return;
            }
            file_count++;
            emit SigUpdateProgress(file_count);
            auto *item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), _root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            RecursiveProTree(fileInfo.absoluteFilePath(), file_count, self, root, item, preItem);
        }
        else //如果是图片文件
        {
            if(_bstop)
            {
                return;
            }

            const QString &suffix = fileInfo.completeSuffix();
            if(suffix != "png" && suffix != "jpeg" && suffix != "jpg")
            {
                continue; //如果不是图片就跳过，不做src_path和dist_path的拷贝
            }
            file_count++;
            emit SigUpdateProgress(file_count);

            auto *item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(), _root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());

            if(preItem)
            {
                auto *pre_proitem = dynamic_cast<ProTreeItem *>(preItem);
                pre_proitem->SetNextItem(item);
            }

            item->SetPreItem(preItem);
            preItem = item;
        }
    }
    emit SigFinishProgress(file_count);
}

void OpenTreeThread::SlotCancelOpenProgress()
{
    this->_bstop = true;
}

