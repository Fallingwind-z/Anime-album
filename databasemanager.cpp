#include "databasemanager.h"
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QSqlRecord> //封装数据库表的字段信息和操作记录的数据
#include <QSqlError>
#include <QSqlQuery>

DatabaseManager::DatabaseManager(QObject *parent)
    : QObject{parent}
{
    // 获取应用程序数据存储的标准路径
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dbPath); // 创建QDir对象用于操作目录
    if(!dir.exists())
    {
        bool enable = dir.mkpath(dbPath); // 如果目录不存在，则递归创建所有必需的父目录
        if(!enable)
        {
            qDebug() << "Failed to create database derectory";
        }
    }
    _path = dbPath + "/album.db";
}

DatabaseManager::~DatabaseManager()
{
    closeDatabase();
}

void DatabaseManager::initDatabase()
{
    if(!openDatabase())
    {
        QMessageBox::critical(0, QObject::tr("Failed to open database:"), _db.lastError().text());
    }
    if (!createTables()) {
        QMessageBox::critical(0, QObject::tr("Failed to create tables:"), _db.lastError().text());
    }
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(_db);
    //R 是C++11引入的原始字符串字面量(Raw String Literal)前缀
    QString sql = R"(CREATE TABLE IF NOT EXISTS album (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        name TEXT NOT NULL,
        path TEXT UNIQUE NOT NULL)
    )";

    if(!query.exec(sql))
    {
        QMessageBox::critical(0, QObject::tr("Failed to create projects table:"), _db.lastError().text());
        return false;
    }
    return true;
}

bool DatabaseManager::openDatabase()
{
    if(_db.isOpen())
    {
        return true; //数据库已经打开
    }
    //QSQLITE 表示使用SQLite数据库驱动, Album 用于唯一标识这个数据库连接
    _db = QSqlDatabase::addDatabase("QSQLITE", "Album"); //创建并添加一个数据库连接到Qt的数据库连接池中
    _db.setDatabaseName(_path);
    if (!_db.open()) {
        qDebug() << "Database open failed:" << _db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::closeDatabase()
{
    if (_db.isOpen()) {
        _db.close();
    }
}

void DatabaseManager::savePro(const QString &name, const QString &path)
{
    QSqlQuery query(_db);
    // 检查项目是否已存在
    QString sql = QString("SELECT path FROM album WHERE path = '%1'").arg(path);
    if (query.exec(sql) && query.next()) {
        // 项目已存在，更新信息
        sql = QString("UPDATE album SET name = '%1' WHERE path = '%2'").arg(name, path);
    } else {
        // 项目不存在，插入新记录
        sql = QString("INSERT INTO album (name, path) VALUES ('%1', '%2')").arg(name, path);
    }

    if (!query.exec(sql)) {
        QMessageBox::critical(0, QObject::tr("Failed to save project:"), _db.lastError().text());
    }
}

QStringList DatabaseManager::getOpenPro()
{
    QStringList lastOpenedPro;
    if (!openDatabase()) {
        return lastOpenedPro;
    }

    QSqlQuery query(_db);
    QString sql = "SELECT path FROM album";
    if(!query.exec(sql))
    {
        QMessageBox::critical(0, QObject::tr("Failed to get open album:"), _db.lastError().text());
    }

    while (query.next()) {
        lastOpenedPro.append(query.value(0).toString()); //从数据库中查询所有项目路径并收集到字符串列表
    }
    return lastOpenedPro;
}

bool DatabaseManager::deletePro()
{
    if (!openDatabase()) {
        return false;
    }
    QSqlQuery query(_db);
    // 清空album表中的所有数据
    if (!query.exec("DELETE FROM album")) {
        qDebug() << "Failed to clear all album:" << _db.lastError().text();
        return false;
    }
    return true;
}
