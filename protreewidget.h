#ifndef PROTREEWIDGET_H
#define PROTREEWIDGET_H

#include <QTreeWidget>
#include <QAction>
#include <QProgressDialog>
#include "protreethread.h"
#include "opentreethread.h"
#include "mediaplaylist.h"
#include <QtMultiMedia/QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>

class SlideShowDialog; //前向声明，避免互引用，可在.cpp中包含具体的头文件

class ProTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    ProTreeWidget(QWidget *parent = nullptr);
    void AddProToTree(const QString &name, const QString &path);
    QStringList getOpenPro();

private:
    QSet<QString> _set_path; //用来判断重名
    QTreeWidgetItem *_right_btn_item; //右键单击某个item之后弹出菜单
    QTreeWidgetItem *_active_item; //存储当前激活条目
    QTreeWidgetItem *_selected_item; //当前选中的条目
    QAction *_action_import;
    QAction *_action_setstart;
    QAction *_action_closepro;
    QAction *_action_slideshow;
    QProgressDialog *_import_progressdlg;
    QProgressDialog *_open_progressdlg;
    std::shared_ptr<ProTreeThread> _thread_create_pro; //用智能指针管理创建项目线程的生命周期
    std::shared_ptr<OpenTreeThread> _thread_open_pro; //用智能指针管理打开项目线程的生命周期
    std::shared_ptr<SlideShowDialog> _slide_show_dlg; //SlideShowDialog的成员变量-智能指针
    QMediaPlayer *_player;
    QAudioOutput *_audioOutput; // Qt6 新增，用于音频输出
    MediaPlaylist *_playlist; // 替代 QMediaPlaylist，存储所有音乐URL
    int _currentIndex;   // 当前播放索引

private slots:
    void SlotItemPressed(QTreeWidgetItem *item, int column);
    void SlotDoubleClickItem(QTreeWidgetItem *doubleitem, int col);
    void SlotImport();
    void SlotSetActive();
    void SlotClosePro();
    void SlotUpdateProgress(int count);
    void SlotFinishProgress();
    void SlotCancelProgress();
    void SlotUpdateOpenProgress(int count);
    void SlotFinishOpenProgress();
    void SlotCancelOpenProgress();
    void SlotSlideShow();

public slots:
    void SlotOpenPro(const QString &path); //需要用public slots，因为是从mainwindow中来的
    void SlotPreShow();
    void SlotNextShow();
    void SlotSetMusic();
    void SlotStartMusic();
    void SlotStopMusic();
    void SlotPauseMusic(); //暂停音乐槽函数
    void SlotResumeMusic(); //恢复音乐槽函数
    void SlotMusicChanged(int index);

signals:
    void SigCancelProgress();
    void SigCancelOpenProgress();
    void SigUpdateSelected(const QString &path);
    void SigUpdatePic(const QString &path);
    void SigClearSelected();
};

#endif // PROTREEWIDGET_H
