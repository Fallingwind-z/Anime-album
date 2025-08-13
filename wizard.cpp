#include "wizard.h"
#include "ui_wizard.h"

Wizard::Wizard(QWidget *parent)
    : QWizard(parent)
    , ui(new Ui::Wizard)
{
    ui->setupUi(this);
}

Wizard::~Wizard()
{
    delete ui;
}

//重写父类的done函数
void Wizard::done(int result)
{
    if(result == QDialog::Rejected)
    {
        return QWizard::done(result); //用户点击 "取消"，直接调用基类的 done(result) 关闭向导，不做额外处理
    }

    QString name, path;
    ui->wizardPage1->GetProSettings(name, path); //从向导的第一页（即ProSetPage）获取用户输入的项目名称（name）和路径（path）
    emit SigProSettings(name, path); //将获取到的项目名称和路径以信号的形式发送出去，供其他组件处理
    QWizard::done(result); //重写基类函数后如果还想用基类的功能，子类再调用一下基类的函数
}
