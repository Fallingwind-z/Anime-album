#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wizard.h"
#include "protree.h"
#include "protreewidget.h"
#include "picshow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1629, 869); //让窗口以这个比例来缩放
    ui->setupUi(this) ;
    //创建文件菜单
    QMenu *menu_file = menuBar()->addMenu(tr("文件(&F)")); //tr是用于标记需要被翻译的字符串，这里&F是设置快捷键，按住alt+F可选择菜单项
    //创建项目动作
    QAction *act_create_pro = new QAction(QIcon(":/icon/createpro.png"), tr("创建项目"), this);
    act_create_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N)); //快捷键CTRL+N
    menu_file->addAction(act_create_pro);
    //打开项目动作
    QAction *act_open_pro = new QAction(QIcon(":/icon/openpro.png"), tr("打开项目"), this);
    act_open_pro->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O)); //快捷键CTRL+O
    menu_file->addAction(act_open_pro);

    //创建设置菜单
    QMenu *menu_set = menuBar()->addMenu(tr("设置(&S)")); //tr是用于标记需要被翻译的字符串，这里&S是设置快捷键，按住alt+S可选择菜单项
    //设置背景音乐动作
    QAction *act_music = new QAction(QIcon(":/icon/music.png"), tr("背景音乐"), this);
    act_music->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_M)); //快捷键CTRL+M
    menu_set->addAction(act_music);

    //连接信号和槽
    connect(act_create_pro, &QAction::triggered, this, &MainWindow::SlotCreatePro);
    connect(act_open_pro, &QAction::triggered, this, &MainWindow::SlotOpenPro);

    //目录树
    _protree = new ProTree();
    ui->proLayout->addWidget(_protree); //_protree是QWidget *类型，但是我们要的是ProTreeWidget *类型
    //a.使用 dynamic_cast 将 _protree(QWidget *类型) 显式转换为 ProTree* 类型、b.再调用 GetTreeWidget() 方法获取内部的 QTreeWidget* 指针
    QTreeWidget *tree_widget = dynamic_cast<ProTree *>(_protree)->GetTreeWidget(); //因为ProTree下有ui->treeWidget
    auto *pro_tree_widget = dynamic_cast<ProTreeWidget *>(tree_widget); //将QTreeWidget *转为ProTreeWidget *
    connect(this, &MainWindow::SigOpenPro, pro_tree_widget, &ProTreeWidget::SlotOpenPro); //连接信号和槽，告诉ProTreeWidget导入了哪个路径

    //显示图片
    _picshow = new PicShow();
    ui->picLayout->addWidget(_picshow);
    auto *pro_pic_show = dynamic_cast<PicShow *>(_picshow);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdateSelected, pro_pic_show, &PicShow::SlotSelectItem);
    connect(pro_pic_show, &PicShow::SigPreClicked, pro_tree_widget, &ProTreeWidget::SlotPreShow);
    connect(pro_pic_show, &PicShow::SigNextClicked, pro_tree_widget, &ProTreeWidget::SlotNextShow);
    connect(pro_tree_widget, &ProTreeWidget::SigUpdatePic, pro_pic_show, &PicShow::SlotUpdatePic);
    connect(pro_tree_widget, &ProTreeWidget::SigClearSelected, pro_pic_show, &PicShow::SlotDeleteItem);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    auto *pro_pic_show = dynamic_cast<PicShow *>(_picshow);
    pro_pic_show->ReloadPic();
    QMainWindow::resizeEvent(event);
}

void MainWindow::SlotCreatePro(bool)
{
    qDebug()<<"slot create pro triggered";
    Wizard wizard(this);
    wizard.setWindowTitle(tr("创建项目"));
    auto *page = wizard.page(0); //第一页
    page->setTitle(tr("设置项目配置"));
    //连接信号和槽，把项目配置传回来
    //_protree是QWidget类型，而AddProToTree是ProTree类型，需要dynamic_cast安全转换
    connect(&wizard, &Wizard::SigProSettings, dynamic_cast<ProTree*>(_protree), &ProTree::AddProToTree);
    wizard.show();
    wizard.exec();
    //断开所有信号
    disconnect(&wizard);
}

void MainWindow::SlotOpenPro(bool)
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setWindowTitle(tr("选择导入的文件夹"));
    file_dialog.setDirectory(QDir::currentPath());
    QStringList fileNames;
    if(file_dialog.exec())
    {
        fileNames = file_dialog.selectedFiles();
    }

    if(fileNames.length() <= 0)
    {
        return;
    }

    QString import_path = fileNames.at(0);
    emit SigOpenPro(import_path); //告诉ProTreeWidget导入了哪个路径
}
