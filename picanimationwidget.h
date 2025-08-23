#ifndef PICANIMATIONWIDGET_H
#define PICANIMATIONWIDGET_H
#include <QWidget>
#include <QTreeWidgetItem>

class PicAnimationWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWidget(QWidget *parent = nullptr);
    ~PicAnimationWidget();
    void SetPixmap(QTreeWidgetItem *item);
    void Start();
    void Stop();
    void SlidePre();
    void SlideNext();

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    void UpdateSelectPixmap(QTreeWidgetItem *item);
    float _factor; //控制动画显示（0-1）
    QTimer *_timer; //不断更新_factor
    QPixmap _pixmap1;
    QPixmap _pixmap2; //双缓冲绘图
    QTreeWidgetItem *_cur_item; //当前播放到哪了
    QMap<QString, QTreeWidgetItem *> _map_items; //
    bool _b_start; //是否开始播放
    bool _b_paused; //是否处于暂停状态

private slots:
    void TimeOut();

public slots:
    void SlotUpdateSelectShow(QString path);
    void SlotStartOrStop();

signals:
    void SigUpdatePreList(QTreeWidgetItem *item); //更新动画缩略图列表
    void SigSelectItem(QTreeWidgetItem *item); //哪一个item处于播放状态
    void SigStart();
    void SigStop();
    void SigStartMusic();
    void SigStopMusic();
    void SigPauseMusic(); //暂停音乐信号
    void SigResumeMusic(); //恢复音乐信号
};

#endif // PICANIMATIONWIDGET_H
