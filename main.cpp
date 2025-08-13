#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/style/style.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug("open qss success!");
        /*
            为什么用 Latin-1？
            Latin-1 是单字节编码，能无损兼容ASCII，且不会像UTF-8那样因多字节字符解析失败（如果QSS文件是纯ASCII或Latin-1编码，用它可以避免编码问题）
        */
        QString style = QLatin1String(qss.readAll()); //读取QSS文件内容并转换为QString,用QLatin1String可以提高效率
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug("open qss failed!");
        return 0;
    }

    MainWindow w;
    w.setWindowTitle("Album"); //窗口标题
    w.showMaximized(); //最大化窗口显示
    return a.exec();
}
