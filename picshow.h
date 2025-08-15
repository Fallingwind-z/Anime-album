#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QGraphicsOpacityEffect> //控制控件的透明度（淡入淡出效果）
#include <QPropertyAnimation> //对对象的属性（如位置、大小、透明度等）进行动画过渡

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = nullptr);
    ~PicShow();
    void ReloadPic();

protected:
    bool event(QEvent *e) override;

private:
    Ui::PicShow *ui;
    QGraphicsOpacityEffect *opacity_pre;
    QGraphicsOpacityEffect *opacity_next;
    QPropertyAnimation *_animation_show_pre;
    QPropertyAnimation *_animation_show_next;
    void ShowPreNextBtns(bool b_show);
    bool _b_btnvisible;
    QString _select_path;
    QPixmap _pix_map;

signals:
    void SigPreClicked();
    void SigNextClicked();

public slots:
    void SlotSelectItem(const QString &path);
    void SlotUpdatePic(const QString &path);
    void SlotDeleteItem();
};

#endif // PICSHOW_H
