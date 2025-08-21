#ifndef MEDIAPLAYLIST_H
#define MEDIAPLAYLIST_H

#include <QObject>
#include <QList>
#include <QUrl>

/*
 * 自定义简易媒体播放列表类
 * 替代 Qt6 中移除的 QMediaPlaylist，实现播放列表的基本管理功能
 */

class MediaPlaylist : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlaylist(QObject *parent = nullptr);

    // 添加一条媒体资源（文件路径或 URL）
    void addMedia(const QUrl &media);

    // 清空播放列表
    void clear();

    // 获取当前播放索引
    int currentIndex() const;

    // 设置当前播放索引（切换到指定索引的媒体）
    void setCurrentIndex(int index);

    // 获取当前播放的媒体 URL
    QUrl currentMedia() const;

    // 切换到下一条媒体，返回下一条媒体的 URL
    QUrl nextMedia();

    // 切换到上一条媒体，返回上一条媒体的 URL
    QUrl previousMedia();

    // 返回播放列表中媒体的数量
    int mediaCount() const;

signals:
    // 当前播放索引改变时触发
    void currentIndexChanged(int newIndex);

private:
    QList<QUrl> _mediaList; // 存储媒体列表
    int _currentIndex;   // 当前播放的索引，-1表示无有效索
};

#endif // MEDIAPLAYLIST_H
