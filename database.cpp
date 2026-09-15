#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool Database::open(const QString &dbPath)
{
    m_dbPath = dbPath;

    // 用 Qt 内置的 SQLite 驱动；addDatabase 返回一个连接（"QSQLITE" 是驱动名）
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);   // SQLite 里这就是数据库文件路径

    if (!db.open()) {
        qWarning() << "打开数据库失败:" << db.lastError().text();
        return false;
    }

    // 建表（IF NOT EXISTS 保证重复打开不报错）
    QSqlQuery query(db);
    const QString sql =
        "CREATE TABLE IF NOT EXISTS tasks ("
        "  id         INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  title      TEXT NOT NULL,"
        "  due_time   TEXT NOT NULL,"
        "  done       INTEGER DEFAULT 0,"
        "  created_at TEXT DEFAULT (datetime('now'))"
        ")";
    if (!query.exec(sql)) {
        qWarning() << "建表失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::addTask(const QString &title, const QString &dueTime)
{
    QSqlDatabase db = QSqlDatabase::database();  // 取默认连接
    QSqlQuery query(db);

    // 用占位符 :title / :due 防 SQL 注入（养成好习惯）
    query.prepare("INSERT INTO tasks (title, due_time) VALUES (:title, :due)");
    query.bindValue(":title", title);
    query.bindValue(":due", dueTime);
    if (!query.exec()) {
        qWarning() << "添加失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::removeTask(int id)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qWarning() << "删除失败:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Database::setTaskDone(int id, bool done)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.prepare("UPDATE tasks SET done = :done WHERE id = :id");
    query.bindValue(":done", done ? 1 : 0);
    query.bindValue(":id", id);
    if (!query.exec()) {
        qWarning() << "更新失败:" << query.lastError().text();
        return false;
    }
    return true;
}

QVector<Task> Database::allTasks()
{
    QVector<Task> tasks;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    query.exec("SELECT id, title, due_time, done, created_at FROM tasks ORDER BY due_time ASC");

    while (query.next()) {
        Task t;
        t.id        = query.value(0).toInt();
        t.title     = query.value(1).toString();
        t.dueTime   = query.value(2).toString();
        t.done      = query.value(3).toInt() != 0;   // SQLite 用 0/1 存布尔
        t.createdAt = query.value(4).toString();
        tasks.append(t);
    }
    return tasks;
}
