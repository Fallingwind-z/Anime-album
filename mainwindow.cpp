#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wizard.h"
#include "protree.h"
#include <QMenu>
#include <QAction>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
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

    _protree = new ProTree();
    ui->proLayout->addWidget(_protree);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    disconnect();
}
