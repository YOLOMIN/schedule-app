#ifndef DATABASE_H
#define DATABASE_H

#include <QString>
#include <QVector>
#include "task.h"

// 数据库管理类：封装 SQLite 的连接、建表、增删改查
// 主窗口只调用这里的方法，不直接写 SQL，实现「数据访问层」与「界面层」分离
class Database
{
public:
    // 打开（不存在则创建）数据库文件，并建表。返回是否成功
    bool open(const QString &dbPath);

    // 增：添加一条日程，返回是否成功
    bool addTask(const QString &title, const QString &dueTime);

    // 删：按 id 删除
    bool removeTask(int id);

    // 改：更新「完成」状态
    bool setTaskDone(int id, bool done);

    // 查：读取全部日程，按到期时间升序排列
    QVector<Task> allTasks();

private:
    QString m_dbPath;
};

#endif // DATABASE_H
