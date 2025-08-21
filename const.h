#ifndef CONST_H
#define CONST_H

enum TreeItemType
{
    TreeItemPro = 1, //表示项目的条目
    TreeItemDir = 2, //表示项目的文件夹
    TreeItemPic = 3, //表示项目的图片
};

enum PicButtonState
{
    PicButtonStatePlayNormal = 1,
    PicButtonStatePlayHover = 2,
    PicButtonStatePlayPress = 3,

    PicButtonStatePauseNormal = 4,
    PicButtonStatePauseHover = 5,
    PicButtonStatePausePress = 6,
};

const int PROGRESS_WIDTH = 300;
const int PROGRESS_MAX = 300;
const int PREITEM_SIZE = 100;
const int PREICON_SIZE = 90;

#endif // CONST_H
