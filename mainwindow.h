#include "databasemanager.h"

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class ProTreeWidget; // 前向声明

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_protree; //用QWidget类型(QTreeWidget的父类)是为了降低类间的耦合性，避免可能的互引用问题
    QWidget *_picshow;
    ProTreeWidget *_proTreeWidget; // ProTreeWidget指针，用于获取打开的项目列表
    DatabaseManager *_dbManager;
    void savePro();
    void restorePro();

protected:
    virtual void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void SlotCreatePro(bool);
    void SlotOpenPro(bool);

public slots:
    void SlotProSettings(const QString name, const QString path);

signals:
    void SigOpenPro(const QString &path);
};

#endif // MAINWINDOW_H
