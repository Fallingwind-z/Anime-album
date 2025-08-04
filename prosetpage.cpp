#include "prosetpage.h"
#include "ui_prosetpage.h"
#include <QDir>
#include <QFileDialog>

ProSetPage::ProSetPage(QWidget *parent)
    : QWizardPage(parent)
    , ui(new Ui::ProSetPage)
{
    ui->setupUi(this);
    registerField("proName*", ui->lineEdit);
    registerField("proPath", ui->lineEdit_2); //注册域，lineEdit_2在没有输入的情况下next按钮是灰色的，如果输入了那就会生效
    //当lineEdit修改时就发送信号，触发virtual bool isComplete() const 函数，判断是否完成
    connect(ui->lineEdit, &QLineEdit::cursorPositionChanged, this, &ProSetPage::completeChanged);
    connect(ui->lineEdit_2, &QLineEdit::cursorPositionChanged, this, &ProSetPage::completeChanged);
    QString curPath = QDir::currentPath();
    ui->lineEdit_2->setText(curPath); //设置默认路径
    ui->lineEdit_2->setCursorPosition(ui->lineEdit_2->text().size()); //调整光标位置
    ui->lineEdit_2->setClearButtonEnabled(true); //清空按钮
    ui->lineEdit->setClearButtonEnabled(true); //清空按钮
}

ProSetPage::~ProSetPage()
{
    delete ui;
}

void ProSetPage::GetProSettings(QString &name, QString &path)
{
    name = ui->lineEdit->text();
    path = ui->lineEdit_2->text();
}

bool ProSetPage::isComplete() const
{
    if(ui->lineEdit->text() == "" || ui->lineEdit_2->text() == "")
    {
        return false;
    }
    QDir dir(ui->lineEdit_2->text());
    if(!dir.exists())
    {
        ui->tips->setText("project path is not exists");
        return false;
    }
    //判断路径
    QString absFilePath = dir.absoluteFilePath(ui->lineEdit->text());
    QDir dist_dir(absFilePath); //绝对路径转为目录
    if(dist_dir.exists())
    {
        ui->tips->setText("project has exists, change path or name!");
        return false;
    }
    ui->tips->setText("");
    return QWizardPage::isComplete(); //重写基类函数后如果还想用基类的功能，子类再调用一下基类的函数
}

void ProSetPage::on_pushButton_clicked()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    auto path = QDir::currentPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail); //打开文件夹的显示模式

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
    ui->lineEdit_2->setText(import_path);
}

