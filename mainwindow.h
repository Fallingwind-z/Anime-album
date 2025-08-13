#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget *_protree; //用QWidget类型(QTreeWidget的父类)是为了降低类间的耦合性，避免可能的互引用问题

private slots:
    void SlotCreatePro(bool);
};

#endif // MAINWINDOW_H
