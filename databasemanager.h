#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();
    void initDatabase();
    bool createTables(); //创建数据库表
    bool openDatabase(); //获取数据库连接
    void closeDatabase(); //关闭数据库连接
    void savePro(const QString &name, const QString &path); //保存项目路径到数据库
    QStringList getOpenPro(); //获取所有打开项目的路径列表
    bool deletePro(); //删除数据库

private:
    QSqlDatabase _db;
    QString _path;

signals:
};

#endif // DATABASEMANAGER_H
