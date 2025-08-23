#ifndef SLIDESHOWDIALOG_H
#define SLIDESHOWDIALOG_H

#include <QDialog>
#include <QTreeWidgetItem>

namespace Ui {
class SlideShowDialog;
}

class SlideShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SlideShowDialog(QWidget *parent = nullptr,
                             QTreeWidgetItem *first_item = nullptr, QTreeWidgetItem *last_item = nullptr); //从first_item图片播放到last_item图片
    ~SlideShowDialog();

private:
    Ui::SlideShowDialog *ui;
    QTreeWidgetItem *_first_item;
    QTreeWidgetItem *_last_item;

public slots:
    void SlotSlidePre();
    void SlotSlideNext();
    void SlotCloseShow(); //新增关闭幻灯片槽函数
};

#endif // SLIDESHOWDIALOG_H
