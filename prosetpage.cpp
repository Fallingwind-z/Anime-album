#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QFileDialog> //文件对话框

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proName*", ui->lineEdit_Name); //注册域，lineEdit_Name在没有输入的情况下Next按钮是灰色的，如果输入了那就会生效。 *后缀表示该字段是必填项
    registerField("proPath", ui->lineEdit_Path); //注册域，lineEdit_Path在没有输入的情况下Next按钮是灰色的，如果输入了那就会生效
    //当lineEdit修改时就发送completeChanged信号，触发virtual bool isComplete() const函数，判断是否完成
    connect(ui->lineEdit_Name, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_Path, &QLineEdit::textEdited, this, &ProSetPage::completeChanged);
    QString curPath = QDir::currentPath(); //获取当前项目路径
    ui->lineEdit_Path->setText(curPath); //设置当前项目路径为默认路径
    ui->lineEdit_Path->setCursorPosition(ui->lineEdit_Path->text().size()); //调整光标位置到输入文本的最后
    ui->lineEdit_Path->setClearButtonEnabled(true); //清空按钮
    ui->lineEdit_Name->setClearButtonEnabled(true); //清空按钮
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

//返回所设置的name和path
void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit_Name->text();
    path = ui->lineEdit_Path->text();
}

//重写从基类继承过来的虚函数
bool ProSetPage::isComplete() const
{
    if(ui->lineEdit_Name->text() == "" || ui->lineEdit_Path->text() == "")
    {
        return false; //都为空不跳转
    }

    //判断项目路径是否存在
    QDir dir(ui->lineEdit_Path->text());
    if(!dir.exists())
    {
        ui->tips->setText("project path is not exists"); //目录不存在
        return false;
    }

    //判断项目是否已存在
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit_Name->text()); //拼接项目名称和路径，生成项目的完整路径
    QDir dist_dir(absFilePath); //绝对路径转为目录
    if(dist_dir.exists())
    {
        ui->tips->setText("project has exists, change path or name!"); //错误，项目已存在！
        return false;
    }
    ui->tips->setText("");
    return QWizardPage::isComplete(); //重写基类函数后如果还想用基类的功能，子类再调用一下基类的函数
}

//browse按钮点击槽函数
void ProSetPage::on_pushButton_browse_clicked()
{
    QFileDialog file_dialog; //创建文件对话框对象
    file_dialog.setFileMode(QFileDialog::Directory); //设置文件打开模式：打开文件目录
    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path); //默认为当前项目路径
    file_dialog.setViewMode(QFileDialog::Detail); //打开文件夹的显示模式：详细模式（显示文件名、大小、修改日期等）

    QStringList fileNames; //存储用户选择的路径,用QStringList是因为selectedFiles返回的是QStringList
    if(file_dialog.exec())
    {
        fileNames = file_dialog.selectedFiles(); //用户点击 "确定" 时，获取用户选择的文件路径
    }
    if(fileNames.length() <= 0)
    {
        return; //如果 fileNames 为空（用户未选择路径或点击取消），直接return
    }

    QString import_path = fileNames.at(0); //从 fileNames 中取出第 0 个元素（用户选择的唯一路径）
    ui->lineEdit_Path->setText(import_path); //将用户选择的路径 import_path 设置到 lineEdit_Path 输入框中，显示给用户
}

