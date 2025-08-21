#include "mediaplaylist.h"

MediaPlaylist::MediaPlaylist(QObject *parent): QObject(parent), _currentIndex(-1)
{

}

void MediaPlaylist::addMedia(const QUrl &media)
{
    // 向列表中添加新的媒体资源
    _mediaList.append(media);
    // 如果之前没有有效索引，新增媒体后将当前索引设为0，并触发信号
    if (_currentIndex == -1) {
        _currentIndex = 0;
        emit currentIndexChanged(_currentIndex);
    }
}

void MediaPlaylist::clear()
{
    // 清空媒体列表，并重置索引
    _mediaList.clear();
    _currentIndex = -1;
    emit currentIndexChanged(_currentIndex);
}

int MediaPlaylist::currentIndex() const
{
    // 返回当前播放索引
    return _currentIndex;
}

void MediaPlaylist::setCurrentIndex(int index)
{
    // 设置当前播放索引，条件是索引有效且与当前不同
    if (index >= 0 && index < _mediaList.size() && index != _currentIndex) {
        _currentIndex = index;
        emit currentIndexChanged(_currentIndex);
    }
}

QUrl MediaPlaylist::currentMedia() const
{
    // 返回当前播放的媒体 URL，如果索引无效返回空 QUrl
    if (_currentIndex >= 0 && _currentIndex < _mediaList.size())
    {
        return _mediaList.at(_currentIndex);
    }
    return QUrl();
}

QUrl MediaPlaylist::nextMedia()
{
    // 切换到下一条媒体，循环播放（到达末尾后返回开头）
    if (_mediaList.isEmpty())
    {
        return QUrl();
    }
    _currentIndex = (_currentIndex + 1) % _mediaList.size();
    emit currentIndexChanged(_currentIndex);
    return _mediaList.at(_currentIndex);
}

QUrl MediaPlaylist::previousMedia()
{
    // 切换到上一条媒体，循环播放（到达开头后返回末尾）
    if (_mediaList.isEmpty())
    {
        return QUrl();
    }
    _currentIndex = (_currentIndex - 1 + _mediaList.size()) % _mediaList.size();
    emit currentIndexChanged(_currentIndex);
    return _mediaList.at(_currentIndex);
}

int MediaPlaylist::mediaCount() const
{
    // 返回播放列表总数
    return _mediaList.size();
}
