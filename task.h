#ifndef TASK_H
#define TASK_H

#include <QString>

// 「一条日程」的数据模型：纯数据类，不含任何界面/数据库逻辑
struct Task
{
    int id = 0;           // 数据库主键（0 表示还未入库）
    QString title;        // 日程标题
    QString dueTime;      // 到期时间，ISO 格式字符串，如 "2026-09-06 14:30:00"
    bool done = false;    // 是否已完成
    QString createdAt;    // 创建时间
};

#endif // TASK_H
